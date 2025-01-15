#include "../include/TemperatureProcessor.h"

TemperatureProcessor::TemperatureProcessor() {}

void TemperatureProcessor::addTemperature(float temperature) {
    hourlyTemps.push_back(temperature);
    dailyTemps.push_back(temperature);
}

float TemperatureProcessor::calculateHourlyAverage() {
    float sum = 0;
    for (float temp : hourlyTemps) {
        sum += temp;
    }
    return sum / hourlyTemps.size();
}

float TemperatureProcessor::calculateDailyAverage() {
    float sum = 0;
    for (float temp : dailyTemps) {
        sum += temp;
    }
    return sum / dailyTemps.size();
}

void TemperatureProcessor::resetHourly() {
    hourlyTemps.clear();
}

void TemperatureProcessor::resetDaily() {
    dailyTemps.clear();
}