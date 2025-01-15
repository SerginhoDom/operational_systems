#include "../include/SerialReader.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif

SerialReader::SerialReader(const std::string &port) : portName(port) {
#ifdef _WIN32
    hSerial = INVALID_HANDLE_VALUE;
#else
    fd = -1;
#endif
}

bool SerialReader::open() {
#ifdef _WIN32
    hSerial = CreateFileA(portName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port\n";
        return false;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state\n";
        return false;
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state\n";
        return false;
    }
#else
    fd = ::open(portName.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening serial port\n";
        return false;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error getting serial port attributes\n";
        return false;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; // 8 data bits

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting serial port attributes\n";
        return false;
    }
#endif
    return true;
}

bool SerialReader::read(float &temperature) {
#ifdef _WIN32
    DWORD bytesRead;
    if (!ReadFile(hSerial, &temperature, sizeof(temperature), &bytesRead, NULL)) {
        std::cerr << "Error reading from serial port\n";
        return false;
    }
#else
    if (::read(fd, &temperature, sizeof(temperature)) != sizeof(temperature)) {
        std::cerr << "Error reading from serial port\n";
        return false;
    }
#endif
    return true;
}

void SerialReader::close() {
#ifdef _WIN32
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
#else
    if (fd != -1) {
        ::close(fd);
        fd = -1;
    }
#endif
}

SerialReader::~SerialReader() {
    close();
}