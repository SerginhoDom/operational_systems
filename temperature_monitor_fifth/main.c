#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_ENTRIES 86400

typedef struct {
    int temperature;
    time_t timestamp;
} TemperatureEntry;

TemperatureEntry temp_log[MAX_ENTRIES];
int log_index = 0;

void log_temperature(int temp) {
    time_t now = time(NULL);
    temp_log[log_index].temperature = temp;
    temp_log[log_index].timestamp = now;
    printf("Записано значение: %d, время: %ld\n", temp, now);
    log_index = (log_index + 1) % MAX_ENTRIES;
}

void calculate_hourly_average(FILE *hourly_log) {
    time_t now = time(NULL);
    time_t hour_ago = now - 3600;
    int sum = 0, count = 0;

    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (temp_log[i].timestamp >= hour_ago && temp_log[i].timestamp <= now) {
            sum += temp_log[i].temperature;
            count++;
        }
    }

    if (count > 0) {
        int avg = sum / count;
        fprintf(hourly_log, "%ld: %d\n", now, avg);
        fflush(hourly_log);
        printf("Средняя температура за час: %d\n", avg);
    }
}

void read_from_port(const char *port_name) {
    FILE *port = fopen(port_name, "r");
    if (!port) {
        perror("Ошибка открытия порта");
        exit(EXIT_FAILURE);
    }

    printf("Порт %s успешно открыт. Ожидание данных...\n", port_name);

    char line[32];
    while (fgets(line, sizeof(line), port)) {
        int temp = atoi(line);
        printf("Прочитано значение: %d\n", temp);
        log_temperature(temp);
    }

    fclose(port);
}

int main() {
    FILE *all_temps_log = fopen("all_temps.log", "a");
    if (!all_temps_log) {
        perror("Ошибка открытия all_temps.log");
        exit(EXIT_FAILURE);
    }
    printf("Лог-файл all_temps.log успешно открыт.\n");

    FILE *hourly_avg_log = fopen("hourly_avg.log", "a");
    if (!hourly_avg_log) {
        perror("Ошибка открытия hourly_avg.log");
        exit(EXIT_FAILURE);
    }
    printf("Лог-файл hourly_avg.log успешно открыт.\n");

    while (1) {
        read_from_port("COM6"); // Основная программа читает данные с COM6
        calculate_hourly_average(hourly_avg_log);
        sleep(3600); // Пауза 1 час
    }

    fclose(all_temps_log);
    fclose(hourly_avg_log);

    return 0;
}