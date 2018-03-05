#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "graphics.h"

void test(vec2*uv, vec3* out) {
    out->x = 1.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}

void app_main() {
    graphics_init();
    graphics_module_start(test);

    //handle web requests, update buffer
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    graphics_shutdown();
}
