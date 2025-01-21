#include "../input_handler/input_handler.h"
#include "../counter/counter.h"
#include <iostream>
#include <string>
#include <thread>

std::atomic<bool> InputHandler::running(true);

void InputHandler::start() {
    std::thread inputThread(handleInput);
    inputThread.detach();
}

void InputHandler::stop() {
    running = false;
}

void InputHandler::handleInput() {
    std::string input;
    while (running) {
        std::cin >> input;
        if (input == "set") {
            int value;
            std::cin >> value;
            Counter::set(value);
        } else if (input == "exit") {
            running = false;
        }
    }
}