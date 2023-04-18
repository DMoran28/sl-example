#include <freertos/FreeRTOS.h>
#include <esp_http_server.h>
#include "http.h"

#define FILE_PATH_MAX 32
#define BUFFER_SIZE   512
typedef struct server_context server_context_t;
struct server_context {
    char base_path[FILE_PATH_MAX];
    char buffer[BUFFER_SIZE];
};

static server_context_t* server_context;
static httpd_handle_t server = NULL;

static esp_err_t handler(httpd_req_t* req) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    httpd_resp_send_chunk(req, "{key: \"abcd1234\"}", 17);
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}

esp_err_t http_start(void) {
    server_context = calloc(1, sizeof(server_context_t));
    if (server_context == NULL) {
        return ESP_ERR_NO_MEM;
    }
    snprintf(server_context->base_path, 2, "/");

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    esp_err_t err = httpd_start(&server, &config);
    if (err != ESP_OK) {
        free(server_context);
        return err;
    }

    httpd_uri_t uri = {0};
    uri.uri = "/data";
    uri.method = HTTP_GET;
    uri.handler = handler;
    uri.user_ctx = server_context;
    httpd_register_uri_handler(server, &uri);
    return ESP_OK;
}