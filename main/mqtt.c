#include <freertos/FreeRTOS.h>
#include <mqtt_client.h>
#include "mqtt.h"

static esp_mqtt_client_handle_t mqtt_client;

static void mqtt_task(void* args) {
    for (;;) {
        esp_mqtt_client_publish(mqtt_client, "/data", "key=abcd1234", 12, 0, 0);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

static void mqtt_event_handler(void* args, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    xTaskCreate(mqtt_task, "mqtt_task", 3072, NULL, 5, NULL);
}

esp_err_t mqtt_start(void) {
    esp_mqtt_client_config_t config = {0};
    config.broker.address.uri = CONFIG_MQTT_BROKER;
    config.credentials.username = CONFIG_MQTT_USER;
    config.credentials.authentication.password = CONFIG_MQTT_PASS;

    mqtt_client = esp_mqtt_client_init(&config);
    if (mqtt_client == NULL) {
        return ESP_FAIL;
    }

    esp_mqtt_client_register_event(mqtt_client, MQTT_EVENT_CONNECTED, &mqtt_event_handler, NULL);
    esp_err_t err = esp_mqtt_client_start(mqtt_client);
    return err;
}