#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void simulate_device(const char *port_name) {
    FILE *port = fopen(port_name, "w");
    if (!port) {
        perror("Ошибка открытия порта");
        exit(EXIT_FAILURE);
    }

    printf("Симулятор запущен. Порт %s открыт.\n", port_name);

    srand(time(NULL));
    while (1) {
        int temperature = 20 + (rand() % 15);
        fprintf(port, "%d\n", temperature);
        fflush(port);
        printf("Отправлено значение: %d\n", temperature);

#ifdef _WIN32
        Sleep(1000); // Пауза 1 секунда (Windows)
#else
        sleep(1); // Пауза 1 секунда (Linux)
#endif
    }

    fclose(port);
}

int main() {
    simulate_device("COM5"); // Симулятор отправляет данные на COM5
    return 0;
}