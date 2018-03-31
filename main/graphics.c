#include "graphics.h"
#include "driver.h"
#include "gamma_correction.h"

#include <string.h> //memset()

static unsigned char* buffer;
static unsigned char* fb;
static unsigned char* bb;

static TaskHandle_t task_handle;

static const char* LOG_TAG = "Graphics";

void graphics_init() {
    driver_init();
    fs_init();

    //combined back and front buffer
    buffer  = (unsigned char*)malloc(2 * BUFFER_SIZE);
    assert(buffer && "failed to allocate front/back buffer");
    memset(buffer, 0, 2 * BUFFER_SIZE);

    bb = buffer;
    fb = buffer + BUFFER_SIZE;
    ESP_LOGI(LOG_TAG, "front/back buffer allocated: fb: %d bb: %d", (int)fb, (int)bb);

    driver_run();
    driver_set_buffer(fb);
}

void graphics_shutdown() {
    driver_shutdown();
    graphics_stop();
    free(buffer);
    fs_shutdown();
    ESP_LOGI(LOG_TAG, "shutdown");
} 

void graphics_swap_buffer() {
    unsigned char* tmp = bb;
    bb = fb;
    fb = tmp;
} 

void sampler_load(sampler_t* sampler) {
    ESP_LOGI(LOG_TAG, "Loading sampler %s", sampler->file);

    sampler->_gif = gd_open_gif(sampler->file);
    assert(sampler->_gif && "gd_open_gif() failed");

    sampler->_buffer = malloc(sampler->_gif->width * sampler->_gif->height * 3);
    assert(sampler->_buffer && "malloc() failed");
}

void sampler_unload(sampler_t* sampler) {
    free(sampler->_buffer);
    gd_close_gif(sampler->_gif);

    ESP_LOGI(LOG_TAG, "Unloaded sampler %s", sampler->file);
}

void sampler_tick(sampler_t* sampler) {
    if (gd_get_frame(sampler->_gif)) {
        gd_render_frame(sampler->_gif, sampler->_buffer);
    } else if (sampler->loop) {
        gd_rewind(sampler->_gif);
        sampler_tick(sampler);
    }

    vTaskDelay(sampler->anim_speed / portTICK_PERIOD_MS); //animation delay
}

void module_task(module_t* module) {
    ESP_LOGI(LOG_TAG, "task running on core %d", xPortGetCoreID());

    if (module->sampler)
        sampler_load(module->sampler);

    for(;;) {
        if (ulTaskNotifyTake(pdTRUE, 0))
            break;

        if (module->sampler)
            sampler_tick(module->sampler);

        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                vec2 uv = { x / 64.0f, y / 32.0f };
                vec4 out = { 0.0f, 0.0f, 0.0f, 1.0f };
                (*((module_func_t)module->fn))(&uv, &out, module->sampler);

                unsigned char* p = bb + ((x + y * 64) * 3);
                float alpha = out.w * 255;
                p[0] = gamma8[(int)(out.x * alpha)];
                p[1] = gamma8[(int)(out.y * alpha)];
                p[2] = gamma8[(int)(out.z * alpha)];
            }
        }
        driver_set_buffer(fb);
        graphics_swap_buffer();
    }

    if (module->sampler)
        sampler_unload(module->sampler);

    ESP_LOGI(LOG_TAG, "task stopped");
    vTaskDelete(NULL);
}

void graphics_run(module_t* module) {
    graphics_stop();
    xTaskCreatePinnedToCore(module_task, 
        "module_task", 10000, (void*) module, 80, &task_handle, 1);
}

void graphics_stop() {
    if (task_handle)
        xTaskNotifyGive(task_handle);

    //TODO: actually wait for task to end
    vTaskDelay(200 / portTICK_PERIOD_MS);
}