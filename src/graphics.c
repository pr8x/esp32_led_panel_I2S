#include "graphics.h"
#include "driver.h"

#include <string.h>

static unsigned char* buffer;
static unsigned char* fb;
static unsigned char* bb;

static TaskHandle_t module_task_handle;

void graphics_init() {
    driver_init();

    //combined back and front buffer
    buffer  = (unsigned char*)malloc(2 * BUFFER_SIZE);
    assert(buffer && "failed to allocate front/back buffer");
    memset(buffer, 0, BUFFER_SIZE);

    bb = buffer;
    fb = buffer + BUFFER_SIZE;
    printf("front/back buffer allocated");

    driver_set_buffer(fb);
    driver_run();
}

void graphics_shutdown() {
    driver_shutdown();
    free(buffer);
} 

void graphics_swap_buffer() {
    unsigned char* tmp = bb;
    bb = fb;
    fb = tmp;
} 

void module_task(void* fn) {
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

void graphics_module_start(module_func_t fn) {
    BaseType_t err = xTaskCreate(module_task, 
        "module_task", 500, (void*) fn, 3, &module_task_handle);
    assert(err == pdPASS && "xTaskCreate() failed.");
}