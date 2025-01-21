#include "../spawner/spawner.h"
#include "../logger/logger.h"
#include "../counter/counter.h"
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

std::atomic<bool> Spawner::running(true);
std::atomic<bool> Spawner::isMaster(false);
std::atomic<bool> Spawner::copy1Running(false);
std::atomic<bool> Spawner::copy2Running(false);

void Spawner::start() {
    std::thread spawnThread(spawnCopies);
    spawnThread.detach();
}

void Spawner::stop() {
    running = false;
}

void Spawner::spawnCopies() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        if (!isMaster) continue;

        if (copy1Running || copy2Running) {
            Logger::log("Previous copies are still running. Skipping this cycle.");
            continue;
        }

        Logger::log("Spawning copies...");

#ifdef _WIN32
        wchar_t copy1[] = L"copy1.exe";
        wchar_t copy2[] = L"copy2.exe";
        STARTUPINFOW si1 = {0}, si2 = {0};
        PROCESS_INFORMATION pi1 = {0}, pi2 = {0};
        si1.cb = sizeof(si1);
        si2.cb = sizeof(si2);

        if (CreateProcessW(NULL, copy1, NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) {
            copy1Running = true;
            WaitForSingleObject(pi1.hProcess, INFINITE);
            CloseHandle(pi1.hProcess);
            CloseHandle(pi1.hThread);
            copy1Running = false;
        }

        if (CreateProcessW(NULL, copy2, NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) {
            copy2Running = true;
            WaitForSingleObject(pi2.hProcess, INFINITE);
            CloseHandle(pi2.hProcess);
            CloseHandle(pi2.hThread);
            copy2Running = false;
        }
#else
        pid_t pid1 = fork();
        if (pid1 == 0) {
            // Copy 1
            Counter::set(Counter::get() + 10);
            Logger::log("Copy 1 PID: " + std::to_string(getpid()) + " Counter: " + std::to_string(Counter::get()));
            exit(0);
        } else if (pid1 > 0) {
            copy1Running = true;
            waitpid(pid1, NULL, 0);
            copy1Running = false;
        }

        pid_t pid2 = fork();
        if (pid2 == 0) {
            // Copy 2
            Counter::set(Counter::get() * 2);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            Counter::set(Counter::get() / 2);
            Logger::log("Copy 2 PID: " + std::to_string(getpid()) + " Counter: " + std::to_string(Counter::get()));
            exit(0);
        } else if (pid2 > 0) {
            copy2Running = true;
            waitpid(pid2, NULL, 0);
            copy2Running = false;
        }
#endif
    }
}