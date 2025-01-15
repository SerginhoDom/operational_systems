#include "../include/Logger.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

Logger::Logger(const std::string &filename) : filename(filename) {}

void Logger::log(const std::string &message) {
    std::ofstream logFile(filename, std::ios::app);
    if (logFile.is_open()) {
        logFile << message << "\n";
    } else {
        std::cerr << "Error opening log file: " << filename << "\n";
    }
}

std::string Logger::getTimestampFromLine(const std::string &line) {
    // Предполагаем, что timestamp находится в начале строки (формат: "YYYY-MM-DD HH:MM:SS")
    return line.substr(0, 19);
}

void Logger::cleanupOldEntries(time_t threshold) {
    std::ifstream inFile(filename);
    std::vector<std::string> lines;
    std::string line;

    // Читаем все строки из файла
    while (std::getline(inFile, line)) {
        std::string timestampStr = getTimestampFromLine(line);
        std::tm tm = {};
        std::istringstream ss(timestampStr);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        time_t entryTime = std::mktime(&tm);

        // Если запись не старше threshold, сохраняем её
        if (entryTime >= threshold) {
            lines.push_back(line);
        }
    }
    inFile.close();

    // Перезаписываем файл
    std::ofstream outFile(filename);
    for (const auto &entry : lines) {
        outFile << entry << "\n";
    }
    outFile.close();
}

Logger::~Logger() {}