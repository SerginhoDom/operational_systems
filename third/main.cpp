#include "logger/logger.h"
#include "counter/counter.h"
#include "spawner/spawner.h"
#include "input_handler/input_handler.h"
#include <thread>
#include <chrono>
#include <csignal>
#include <fstream>
#include <filesystem>

bool isMasterProcess() {
    std::ifstream lockfile("lockfile");
    if (lockfile.good()) {
        return false;
    } else {
        std::ofstream lockfile("lockfile");
        return true;
    }
}

std::atomic<bool> running(true);

void signalHandler(int signal) {
    running = false;
}

void cleanup() {
    std::filesystem::remove("lockfile");
}

int main() {
    std::atexit(cleanup); // Удалить файл блокировки при завершении программы

    if (isMasterProcess()) {
        Spawner::isMaster = true;
        Logger::log("This process is the master.");
    } else {
        Logger::log("This process is a slave.");
    }
    std::signal(SIGINT, signalHandler);

    Logger::log("Program started. PID: " + std::to_string(getpid()));

    std::thread incrementThread([]() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            Counter::increment();
        }
    });

    std::thread logThread([]() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto now = std::chrono::system_clock::now();
            auto now_time_t = std::chrono::system_clock::to_time_t(now);
            Logger::log("Time: " + std::string(std::ctime(&now_time_t)) + " PID: " + std::to_string(getpid()) + " Counter: " + std::to_string(Counter::get()));
        }
    });

    Spawner::start();
    InputHandler::start();

    incrementThread.join();
    logThread.join();

    Logger::log("Program exited. PID: " + std::to_string(getpid()));

    return 0;
}