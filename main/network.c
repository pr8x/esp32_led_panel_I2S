#include "network.h"
#include <stdlib.h>
#include <string.h>

#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

static http_server_t server;
static EventGroupHandle_t wifi_event_group;
static const int WIFI_CONNECTED_BIT = BIT0;
static request_callback_t request_callback;

static const char* LOG_TAG = "Network";

void network_set_callback(request_callback_t req_cb) {
    request_callback = req_cb;
}

static void req_handler_GET(http_context_t http_ctx, void* arg) 
{
    if (request_callback)
        request_callback(&http_ctx);

    FILE *f = fopen("/spiffs/web/index.html", "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* data = malloc(fsize);
    fread(data, fsize, 1, f);

    http_response_begin(http_ctx, 200, "text/html", HTTP_RESPONSE_SIZE_UNKNOWN);
    const http_buffer_t buf = {
        .data = data,
        .data_is_persistent = true
    };
    http_response_write(http_ctx, &buf);
    http_response_end(http_ctx);

    fclose(f);
    free(data);
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(LOG_TAG, "SYSTEM_EVENT_STA_START");
        ESP_ERROR_CHECK(esp_wifi_connect());
    break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(LOG_TAG, "SYSTEM_EVENT_STA_GOT_IP");
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(LOG_TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
        ESP_ERROR_CHECK(esp_wifi_connect());
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
    break;
    default:
        break;
    }
    return ESP_OK;
}

void init_wifi()
{
    esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    ESP_ERROR_CHECK(ret);
    
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    
    tcpip_adapter_init();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = NETWORK_SSID,
            .password = NETWORK_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, 0, 0, portMAX_DELAY);
}

void network_init() {
    init_wifi();

    http_server_options_t options = HTTP_SERVER_OPTIONS_DEFAULT();
    ESP_ERROR_CHECK(http_server_start(&options, &server));
    ESP_ERROR_CHECK(http_register_handler(
        server, "/", HTTP_GET, HTTP_HANDLE_RESPONSE, req_handler_GET, NULL));
}


void network_shutdown() {
    ESP_LOGI(LOG_TAG, "shutdown");
    ESP_ERROR_CHECK(http_server_stop(server));

    // ESP_ERROR_CHECK(esp_wifi_stop());
    // ESP_ERROR_CHECK(esp_wifi_deinit());
    // vEventGroupDelete(wifi_event_group); 
}