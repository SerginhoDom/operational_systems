#ifndef TEMPERATUREPROCESSOR_H
#define TEMPERATUREPROCESSOR_H

#include <vector>

class TemperatureProcessor {
public:
    TemperatureProcessor();
    void addTemperature(float temperature);
    float calculateHourlyAverage();
    float calculateDailyAverage();
    void resetHourly();
    void resetDaily();

private:
    std::vector<float> hourlyTemps;
    std::vector<float> dailyTemps;
};

#endif