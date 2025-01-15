#include <iostream>
#include <sqlite3.h>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int main() {
    // Открываем виртуальный COM-порт (COM6)
    HANDLE hSerial = CreateFileA(
        "COM6",               // Имя порта
        GENERIC_READ,         // Режим доступа (только чтение)
        0,                    // Режим совместного доступа (0 — эксклюзивный доступ)
        NULL,                 // Атрибуты безопасности
        OPEN_EXISTING,        // Флаг открытия (только существующий порт)
        FILE_ATTRIBUTE_NORMAL,// Атрибуты файла
        NULL                  // Шаблон файла (не используется)
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port: COM6\n";
        std::cerr << "Error code: " << GetLastError() << "\n";
        return 1;
    }

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

    // Подключение к базе данных
    sqlite3 *db;
    if (sqlite3_open("temperature_logs.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database\n";
        return 1;
    }

    // Основной цикл
    float temperature;
    DWORD bytesRead;
    while (true) {
        if (ReadFile(hSerial, &temperature, sizeof(temperature), &bytesRead, NULL)) {
            std::string timestamp = getCurrentTimestamp();
            std::string sql = "INSERT INTO logs (temperature, timestamp) VALUES (" + std::to_string(temperature) + ", '" + timestamp + "');";
            if (sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
                std::cerr << "Error inserting data into database\n";
            }
            std::cout << "Received temperature: " << temperature << " C\n"; // Отладочное сообщение
        } else {
            std::cerr << "Error reading from serial port\n";
            break;
        }
        Sleep(1000); // Задержка в 1 секунду
    }

    // Закрытие порта и базы данных
    CloseHandle(hSerial);
    sqlite3_close(db);
    return 0;
}