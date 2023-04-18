idf_component_register(SRCS "main.c" "http.c" "mqtt.c" "wifi.c"
                       INCLUDE_DIRS "include"
                       REQUIRES "esp_http_server" "esp_wifi" "mqtt" "nvs_flash")