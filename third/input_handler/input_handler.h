#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <atomic>

class InputHandler {
public:
    static void start();
    static void stop();

private:
    static void handleInput();

    static std::atomic<bool> running;
};

#endif