# Simple C++ Shell Implementation

## Overview

This is a basic command-line shell written in C++ that demonstrates fundamental Unix system programming concepts. The shell can execute external commands, handle process creation, and manage command execution in a Unix-like environment.

## Features

- **Command Prompt**: Displays `my_shell$` prompt for user input
- **Command Execution**: Executes system commands using `fork()` and `execvp()`
- **Argument Parsing**: Properly handles command arguments separated by spaces
- **Exit Handling**: Supports `exit` command and Ctrl+D to terminate
- **Exit Status**: Displays the exit code of executed commands
- **Error Handling**: Reports command not found errors and fork failures

## How It Works

### 1. Input Reading
```cpp
std::getline(std::cin, input)  // Reads user input
```
- Reads complete lines of input including spaces
- Handles EOF (Ctrl+D) gracefully

### 2. Command Parsing
```cpp
std::istringstream iss(input);
while (iss >> token) {
    args.push_back(token);
}
```
- Splits input string into tokens using spaces as delimiters
- Stores commands and arguments in a `std::vector<std::string>`

### 3. Argument Conversion for execvp()
```cpp
std::vector<char*> exec_args;
for (auto& arg : args) {
    exec_args.push_back(&arg[0]);  // Get pointer to C-style string
}
exec_args.push_back(nullptr);      // NULL-terminate the array
```
- Converts C++ strings to C-style character arrays required by `execvp()`
- Creates a NULL-terminated array of character pointers

### 4. Process Creation and Execution
```cpp
pid_t pid = fork();  // Create child process

if (pid == 0) {
    // Child process: replace with new program
    execvp(exec_args[0], exec_args.data());
    // If execvp returns, an error occurred
    std::cerr << "ERROR: Command not found: " << args[0] << std::endl;
    exit(1);
}
```
- `fork()` creates an identical copy of the current process
- `execvp()` replaces the child process with the requested program
- If `execvp()` fails, the child process reports the error and exits

### 5. Parent Process Management
```cpp
else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);  // Wait for child to complete
    
    if (WIFEXITED(status)) {
        std::cout << "Exit code: " << WEXITSTATUS(status) << std::endl;
    }
}
```
- Parent process waits for the child to complete execution
- Extracts and displays the exit status of the child process

## Build and Run

### Compilation
```bash
g++ -o my_shell shell.cpp -std=c++11
```

### Execution
```bash
./my_shell
```

### Example Usage
```
my_shell$ ls -l
total 24
-rwxr-xr-x 1 user user 16856 Jan 1 12:00 my_shell
-rw-r--r-- 1 user user  1234 Jan 1 12:00 shell.cpp
Exit code: 0

my_shell$ echo "Hello World"
Hello World
Exit code: 0

my_shell$ nonexistent-command
ERROR: Command not found: nonexistent-command
Exit code: 1

my_shell$ exit
```

## Technical Details

### System Calls Used
- `fork()`: Creates a new process by duplicating the calling process
- `execvp()`: Replaces the current process image with a new one
- `waitpid()`: Waits for a child process to change state

### Memory Management
- Uses C++ containers (`std::vector`, `std::string`) for automatic memory management
- Properly handles C-style string conversion for system call compatibility

### Error Handling
- Checks for fork failures
- Handles command not found errors from execvp()
- Validates input before processing

## Limitations

1. **No Built-in Commands**: Cannot handle shell built-ins like `cd`, `export`
2. **No Pipes/Redirection**: Doesn't support `|`, `>`, `<` operators
3. **No Signal Handling**: Ctrl+C terminates the entire shell
4. **No Background Processes**: All commands run in foreground
5. **No Command History**: Doesn't remember previous commands

## Educational Value

This code demonstrates:
- Process creation and management with `fork()` and `exec()`
- Inter-process communication and synchronization
- Command parsing and argument handling
- System call error handling
- Differences between C++ strings and C-style strings
- Basic shell architecture and operation

## Dependencies

- C++11 compatible compiler
- Unix-like operating system (Linux, macOS, BSD)
- Standard C++ library and POSIX headers

## License

This is educational code. Feel free to use and modify for learning purposes.
