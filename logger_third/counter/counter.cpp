#include "../counter/counter.h"

std::atomic<int> Counter::counter(0);
std::mutex Counter::counterMutex;

void Counter::increment() {
    std::lock_guard<std::mutex> lock(counterMutex);
    counter++;
}

void Counter::set(int value) {
    std::lock_guard<std::mutex> lock(counterMutex);
    counter = value;
}

int Counter::get() {
    return counter.load();
}