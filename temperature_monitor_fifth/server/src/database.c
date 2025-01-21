#include <sqlite3.h>
#include <stdio.h>
#include "../include/database.h"

sqlite3 *db;

void init_db() {
    int rc = sqlite3_open("../database/temperature.db", &db);
    if (rc) {
        fprintf(stderr, "Ошибка открытия базы данных: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS logs ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "temperature INTEGER, "
                      "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
    char *err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void save_temperature(int temperature) {
    char sql[128];
    snprintf(sql, sizeof(sql), "INSERT INTO logs (temperature) VALUES (%d);", temperature);

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

const char *get_statistics() {
    // Пример реализации (можно расширить)
    return "{\"average\": 25, \"min\": 20, \"max\": 30}";
}

void close_db() {
    sqlite3_close(db);
}