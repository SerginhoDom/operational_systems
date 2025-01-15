#ifndef BACKGROUND_LAUNCHER_H
#define BACKGROUND_LAUNCHER_H

#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

class BackgroundLauncher {
public:
    static int launch(const std::string& program, const std::string& arguments);
    static bool waitForCompletion(int processId, int& exitCode);
};

#endif // BACKGROUND_LAUNCHER_H