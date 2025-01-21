#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/http_server.h"
#include "../include/database.h"
#include "../include/temperature.h"

int main() {
    // Инициализация базы данных
    init_db();

    // Запуск HTTP-сервера
    start_http_server();

    // Симуляция получения температуры
    while (1) {
        int temperature = get_temperature();
        save_temperature(temperature);
        printf("Текущая температура: %d\n", temperature);

#ifdef _WIN32
        _sleep(1000); // Пауза 1 секунда (Windows)
#else
        sleep(1); // Пауза 1 секунда (Linux)
#endif
    }

    close_db();
    return 0;
}