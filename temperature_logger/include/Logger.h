#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    Logger(const std::string &filename);
    ~Logger();
    void log(const std::string &message);
    void cleanupOldEntries(time_t threshold);

private:
    std::string filename;
    std::string getTimestampFromLine(const std::string &line);
};

#endif