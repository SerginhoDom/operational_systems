#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

int main() {
    std::srand(std::time(0));

    // Открываем виртуальный COM-порт (COM5)
    HANDLE hSerial = CreateFileA(
        "COM5",               // Имя порта
        GENERIC_WRITE,        // Режим доступа (только запись)
        0,                    // Режим совместного доступа (0 — эксклюзивный доступ)
        NULL,                 // Атрибуты безопасности
        OPEN_EXISTING,        // Флаг открытия (только существующий порт)
        FILE_ATTRIBUTE_NORMAL,// Атрибуты файла
        NULL                  // Шаблон файла (не используется)
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port: COM5\n";
        std::cerr << "Error code: " << GetLastError() << "\n"; // Добавлено
        return 1;
    }
    std::cout << "Serial port opened successfully: COM5\n"; // Добавлено

    // Настройка параметров порта
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state\n";
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600; // Скорость 9600 бод
    dcbSerialParams.ByteSize = 8;       // 8 бит данных
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 стоп-бит
    dcbSerialParams.Parity = NOPARITY;  // Без контроля четности

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state\n";
        CloseHandle(hSerial);
        return 1;
    }

    // Основной цикл
    while (true) {
        float temperature = 20.0 + (std::rand() % 100) / 10.0; // Генерация случайной температуры
        DWORD bytesWritten;
        if (!WriteFile(hSerial, &temperature, sizeof(temperature), &bytesWritten, NULL)) {
            std::cerr << "Error writing to serial port\n";
            break;
        }
        std::cout << "Data sent: " << temperature << " C\n"; // Отладочное сообщение
        Sleep(1000); // Задержка в 1 секунду
    }

    // Закрытие порта
    CloseHandle(hSerial);
    return 0;
}