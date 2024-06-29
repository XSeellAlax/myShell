#include "utils.h"
#include <cstdio>
int main() {
    std::string line;
    std::cout << "\033[1;33m-------------------------------------------------\n";
    std::cout << "\033[1;33m|         This project is by 王鹏 王修悟:         |\n";
    std::cout << "\033[1;33m-------------------------------------------------\n";
    while (true) {
        std::cout << "myshell> ";

        if (!std::getline(std::cin, line) || line == "exit") {
        
            break;
        }
        parseAndExecute(line);
    }
    return 0;
}
