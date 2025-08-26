#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <sstream>
int main() {
    std::string input;
    while (true) {
        std::cout << "My shell$ ";
        if (!std::getline(std::cin, input) | input == "exit") break;
        if (input.empty()) continue;
        std::vector<std::string> argc;
        std::istringstream iss(input);
        std::string token;
        while (iss >> token) {
            argc.push_back(token);
        }
        if (argc.empty()) continue;
        std::vector<char*> argv;
        for (auto& arg : argc) {
            argv.push_back(&arg[0]);
        }
        argv.push_back(nullptr);
        pid_t pid = fork();
        if (pid == 0) {
            std::cout << "Current copy pid: " << getpid() << "\n";
            execvp(argv[0], argv.data());
            std::cerr << "ERROR: Command not found\n";
            return 1;
        } else if (pid > 0) {
            int status;
            std::cout << "Current main pid: " << getpid() << "\n";
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                std::cout << "Exited code: " << WEXITSTATUS(status) << "\n";
            }
        } else {
            std::cerr << "ERROR: Comand failed\n";
        }
    }
    return 0;
}