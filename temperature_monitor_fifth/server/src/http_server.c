#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include "../include/http_server.h"
#include "../include/database.h"
#include "../include/temperature.h"

static int handle_request(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {
    const char *page = "<html><body><h1>Temperature Server</h1></body></html>";
    struct MHD_Response *response;
    int ret;

    if (strcmp(method, "GET") == 0) {
        if (strcmp(url, "/temperature") == 0) {
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "{\"temperature\": %d}", get_temperature());
            response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        } else if (strcmp(url, "/statistics") == 0) {
            const char *stats = get_statistics();
            response = MHD_create_response_from_buffer(strlen(stats), stats, MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        } else {
            response = MHD_create_response_from_buffer(strlen(page), (void *)page, MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        }
    } else {
        response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_METHOD_NOT_ALLOWED, response);
    }

    MHD_destroy_response(response);
    return ret;
}

void start_http_server() {
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, 8080, NULL, NULL,
                              &handle_request, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        fprintf(stderr, "Ошибка запуска сервера\n");
        exit(1);
    }
    printf("Сервер запущен на порту 8080\n");
}