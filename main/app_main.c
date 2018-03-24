#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "graphics.h"
#include <math.h>

static float _k = 0.0f;

void module_gif(vec2*uv, vec4* out, sampler_t* sampler) {
    sample(sampler, *uv, (vec3*) out);
}

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

void app_main() {
    graphics_init();

    sampler_t sampler = {
        .file = "/spiffs/box.gif",
        .loop = true
    };

    module_t module = {
        .fn = module_gif,
        .sampler = &sampler
    };
    graphics_run(&module);

    //handle web requests, update buffer
    vTaskDelay(1000000000 / portTICK_PERIOD_MS);

    graphics_shutdown();
}
