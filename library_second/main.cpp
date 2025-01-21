#include "background_launcher.h"
#include <iostream>

int main() {
    std::string program = "sleep";
    std::string arguments = "5"; // Sleep for 5 seconds

    int processId = BackgroundLauncher::launch(program, arguments);
    if (processId == -1) {
        std::cerr << "Failed to launch process" << std::endl;
        return 1;
    }

    std::cout << "Process launched with PID: " << processId << std::endl;

    int exitCode;
    if (BackgroundLauncher::waitForCompletion(processId, exitCode)) {
        std::cout << "Process completed with exit code: " << exitCode << std::endl;
    } else {
        std::cerr << "Failed to wait for process completion" << std::endl;
        return 1;
    }

    return 0;
}