#include "http.h"
#include "mqtt.h"
#include "wifi.h"

void app_main(void) {
    wifi_start();
    mqtt_start();
    http_start();
}