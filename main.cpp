#include "utils.h"

int main() {
    std::string line;
    while (true) {
        std::cout << "myshell> ";

        if (!std::getline(std::cin, line) || line == "exit") {
        
            break;
        }
        parseAndExecute(line);
    }
    return 0;
}
