#include "filesystem.h"
#include "esp_spiffs.h"
#include "common.h"

const char* LOG_TAG = "FileSystem";

void fs_init() {
    ESP_LOGI(LOG_TAG, "Initializing SPIFFS");
    
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };
    
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    size_t total = 0, used = 0;
    ESP_ERROR_CHECK(esp_spiffs_info(NULL, &total, &used));
    ESP_LOGI(LOG_TAG, "Partition size: total: %d, used: %d", total, used);
}

void fs_shutdown() {
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(LOG_TAG, "shutdown");
}