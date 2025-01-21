#include <stdlib.h>
#include <time.h>
#include "../include/temperature.h"

int get_temperature() {
    return 20 + (rand() % 15); // Генерация случайной температуры
}