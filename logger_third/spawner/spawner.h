#ifndef SPAWNER_H
#define SPAWNER_H

#include <atomic>
#include <mutex>

class Spawner {
public:
    static void start();
    static void stop();
    static std::atomic<bool> isMaster;

private:
    static void spawnCopies();

    static std::atomic<bool> running;
    static std::atomic<bool> copy1Running;
    static std::atomic<bool> copy2Running;
};

#endif