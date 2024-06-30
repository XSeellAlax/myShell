#include "parseAndExecute.h"
#include "split.h"

// 解析命令行
void parseAndExecute(std::string commandLine) {
    std::istringstream iss(commandLine);
    std::string segment;
    std::vector<std::string> segments;
    while (std::getline(iss, segment, '|')) {
        segments.push_back(segment);
    }

    size_t num_commands = segments.size();
    int prev_fd[2] = {-1, -1};

    for (size_t i = 0; i < num_commands; ++i) {
        std::string command = segments[i];
        std::vector<std::string> parts = split(command, ' ');
        std::vector<std::string> args;
        std::string infile, outfile;

        for (size_t j = 0; j < parts.size(); ++j) {
            if (parts[j] == "<" && j + 1 < parts.size()) {
                infile = parts[++j];
            } else if (parts[j] == ">" && j + 1 < parts.size()) {
                outfile = parts[++j];
            } else {
                args.push_back(parts[j]);
            }
        }

        int fd[2];
        if (i < num_commands - 1) {  // Not the last command, create a pipe
            pipe(fd);
        }

        pid_t pid = fork();
        if (pid == 0) {
            if (!infile.empty()) {
                int in_fd = open(infile.c_str(), O_RDONLY);
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i > 0) {  // Not the first command
                dup2(prev_fd[0], STDIN_FILENO);
                close(prev_fd[0]);
                close(prev_fd[1]);
            }
            if (i < num_commands - 1) {  // Not the last command
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }
            if (!outfile.empty()) {
                int out_fd = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
            std::vector<char*> c_args;
            for (auto& arg : args) {
                c_args.push_back(&arg[0]);
            }
            c_args.push_back(nullptr);
            execvp(c_args[0], c_args.data());
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            if (i > 0) {
                close(prev_fd[0]);
                close(prev_fd[1]);
            }
            if (i < num_commands - 1) {
                prev_fd[0] = fd[0];
                prev_fd[1] = fd[1];
            }
            waitpid(pid, nullptr, 0);
        } else {
            std::cerr << "Failed to fork." << std::endl;
        }
    }
}
