#include "driver.h"

void app_main() {
    driver_init();

    unsigned char* buffer = (unsigned char*)malloc(BUFFER_SIZE);
    for(int i = 0; i < BUFFER_SIZE; ++i)
        buffer[i] = i % 3;

    driver_set_buffer(buffer);
    driver_run();

    //handle web requests, update buffer
    delay(1000);

    driver_cleanup();
}