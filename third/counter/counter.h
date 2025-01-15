#ifndef COUNTER_H
#define COUNTER_H

#include <atomic>
#include <mutex>

class Counter {
public:
    static void increment();
    static void set(int value);
    static int get();

private:
    static std::atomic<int> counter;
    static std::mutex counterMutex;
};

#endif