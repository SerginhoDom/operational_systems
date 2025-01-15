#include "background_launcher.h"
#include <vector>
#include <sstream>
#include <cstring>

#ifdef _WIN32

int BackgroundLauncher::launch(const std::string& program, const std::string& arguments) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::string cmdLine = program + " " + arguments;

    if (!CreateProcess(
            NULL,                   // No module name (use command line)
            &cmdLine[0],            // Command line
            NULL,                   // Process handle not inheritable
            NULL,                   // Thread handle not inheritable
            FALSE,                  // Set handle inheritance to FALSE
            0,                      // No creation flags
            NULL,                   // Use parent's environment block
            NULL,                   // Use parent's starting directory 
            &si,                    // Pointer to STARTUPINFO structure
            &pi)                    // Pointer to PROCESS_INFORMATION structure
    ) {
        return -1; // Failed to create process
    }

    CloseHandle(pi.hThread);
    return pi.dwProcessId;
}

bool BackgroundLauncher::waitForCompletion(int processId, int& exitCode) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        return false;
    }

    DWORD dwExitCode;
    WaitForSingleObject(hProcess, INFINITE);
    GetExitCodeProcess(hProcess, &dwExitCode);
    CloseHandle(hProcess);

    exitCode = static_cast<int>(dwExitCode);
    return true;
}

#else

int BackgroundLauncher::launch(const std::string& program, const std::string& arguments) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        std::vector<std::string> args;
        std::istringstream iss(arguments);
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(program.c_str()));
        for (const auto& a : args) {
            argv.push_back(const_cast<char*>(a.c_str()));
        }
        argv.push_back(nullptr);

        execvp(program.c_str(), argv.data());
        _exit(127); // If execvp fails
    } else if (pid > 0) {
        // Parent process
        return pid;
    } else {
        // Fork failed
        return -1;
    }
}

bool BackgroundLauncher::waitForCompletion(int processId, int& exitCode) {
    int status;
    waitpid(processId, &status, 0);
    if (WIFEXITED(status)) {
        exitCode = WEXITSTATUS(status);
        return true;
    }
    return false;
}

#endif