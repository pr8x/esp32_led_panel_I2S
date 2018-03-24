#include "filesystem.h"
#include "esp_spiffs.h"
#include "common.h"

static const char* LOG_TAG = "FileSystem";

void fs_init() {
    ESP_LOGI(LOG_TAG, "Initializing SPIFFS");
    
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };
    
    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(LOG_TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(LOG_TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(LOG_TAG, "Failed to initialize SPIFFS (%d)", ret);
        }
        return;
    }
    
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(LOG_TAG, "Failed to get SPIFFS partition information");
    } else {
        ESP_LOGI(LOG_TAG, "Partition size: total: %d, used: %d", total, used);
    }
}

void fs_shutdown() {
    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(LOG_TAG, "SPIFFS unmounted");
}
