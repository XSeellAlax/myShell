#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
extern std::vector<std::string> split(const std::string &s, char delim);

extern void executeCommand(const std::vector<std::string>& args, const std::string& infile, const std::string& outfile);

// 解析命令行
extern void parseAndExecute(std::string commandLine);

#endif