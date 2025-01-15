cmake_minimum_required(VERSION 3.10)
project(TemperatureLogger)

set(CMAKE_CXX_STANDARD 17)

# Добавление основной программы
add_executable(TemperatureLogger
    src/main.cpp
    src/SerialReader.cpp
    src/Logger.cpp
    src/TemperatureProcessor.cpp
)

# Добавление симулятора устройства
add_executable(DeviceSimulator
    DeviceSimulator.cpp
)