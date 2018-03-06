#include "graphics.h"
#include "driver.h"
#include "common.h"

#include <string.h> //memset()

static unsigned char* buffer;
static unsigned char* fb;
static unsigned char* bb;

static TaskHandle_t module_task_handle;
static const char* LOG_TAG = "Graphics";

void graphics_init() {
    driver_init();

    //combined back and front buffer
    buffer  = (unsigned char*)malloc(2 * BUFFER_SIZE);
    assert(buffer && "failed to allocate front/back buffer");
    memset(buffer, 0, 2 * BUFFER_SIZE);

    bb = buffer;
    fb = buffer + BUFFER_SIZE;
    ESP_LOGI(LOG_TAG, "front/back buffer allocated: fb: %d bb: %d", (int)fb, (int)bb);

    driver_set_buffer(fb);
    driver_run();
}

void graphics_shutdown() {
    driver_shutdown();
    free(buffer);
    ESP_LOGI(LOG_TAG, "shutdown");
} 

void graphics_swap_buffer() {
    unsigned char* tmp = bb;
    bb = fb;
    fb = tmp;
} 

void module_task(void* fn) {
    ESP_LOGI(LOG_TAG, "module started");

    for(;;) {
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                vec2 uv;
                uv.x =  x / 64.0f;
                uv.y =  y / 32.0f;

                vec3 out;
                (*((module_func_t)fn))(&uv, &out);

                unsigned char* p = bb + ((x + y * 64) * 3);
                p[0] = (int)(out.x * 255);
                p[1] = (int)(out.y * 255);
                p[2] = (int)(out.z * 255);
            }
        }
        driver_set_buffer(fb);
        graphics_swap_buffer();
    }
}

void graphics_module_start(module_func_t fn) {
    assert(xTaskCreate(module_task, 
        "module_task", 1000, (void*) fn, 4,
        &module_task_handle) && "xTaskCreate() failed.");
}