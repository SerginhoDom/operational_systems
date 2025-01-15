#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <string>

class SerialReader {
public:
    SerialReader(const std::string &port);
    ~SerialReader();
    bool open();
    bool read(float &temperature);
    void close();

private:
    std::string portName;
#ifdef _WIN32
    void* hSerial; // HANDLE на Windows
#else
    int fd; // Файловый дескриптор на Linux/macOS
#endif
};

#endif