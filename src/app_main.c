#include "driver.h"

void app_main() {
    driver_init();

    unsigned char* buffer = (unsigned char*)malloc(BUFFER_SIZE);
    for(int i = 0; i < BUFFER_SIZE; i+=3) {
        buffer[i]   = 0x0;
        buffer[i+1] = 0xFF;
        buffer[i+2] = 0x0;
    }

    driver_set_buffer(buffer);
    driver_run();

    //handle web requests, update buffer
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    driver_shutdown();
}