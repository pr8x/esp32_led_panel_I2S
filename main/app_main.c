#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "graphics.h"
#include "network.h"
#include <math.h>
#include <string.h>

static module_t module; 
static sampler_t sampler;

void module_gif(vec2*uv, vec4* out, sampler_t* sampler) {
    sample(sampler, *uv, (vec3*) out);
}

// void module_solid(vec2*uv, vec4* out, sampler_t* sampler) {
//     out->z = 1.0f;
// }

// void module_sphere(vec2*uv, vec4* out, sampler_t* sampler) {
//     vec2 cuv = { uv->x * 2.0f - 1.0f, uv->y * 2.0f - 1.0f };
//     cuv.x *= 2.0f;
//     cuv.x -= sin(_k+=0.00001f);
//     out->x = length2(cuv);
// }

// void module_plasma(vec2*uv, vec4* out, sampler_t* sampler) {
//     _k += 0.0001f;
//     float iTime = _k;
//     // v1
//     float v1 = sin(uv->x*5.0 + iTime);
       
//     // v2
//     float v2 = sin(5.0*(uv->x*sin(iTime / 2.0) + uv->y*cos(iTime/3.0)) + iTime);
    
//     // v3
//     float cx = uv->x + sin(iTime / 5.0)*5.0;
//     float cy = uv->y + sin(iTime / 3.0)*5.0;
//     float v3 = sin(sqrt(100.0*(cx*cx + cy*cy)) + iTime);
    
//     float vf = v1 + v2 + v3;
//     float r  = cos(vf*M_PI);
//     float g  = sin(vf*M_PI + 6.0*M_PI/3.0);
//     float b  = cos(vf*M_PI + 4.0*M_PI/3.0);

//     out->x = r;
//     out->y = g;
//     out->z = b;
// }

static void network_request(http_context_t* ctx) {
    const char* arg_load = http_request_get_arg_value(*ctx, "load_gif");
    if (arg_load != NULL) {
        ESP_LOGI("Main", "requested file: %s", arg_load);

        strcpy(sampler.file, arg_load);
        sampler.loop = true;

        module.fn = module_gif;
        module.sampler = &sampler;
        sampler.anim_speed = 16;

        graphics_run(&module);
    }

    const char* arg_anim_speed = http_request_get_arg_value(*ctx, "anim_speed");
    if (arg_anim_speed != NULL) {
        ESP_LOGI("Main", "requested animation speed: %s", arg_anim_speed);

        sampler.anim_speed = atoi(arg_anim_speed);
    }
}

void app_main() {

    graphics_init();
    network_init();
    network_set_callback(network_request);

    vTaskDelay(50000000 / portTICK_PERIOD_MS);

    graphics_shutdown();
    network_shutdown();
}
