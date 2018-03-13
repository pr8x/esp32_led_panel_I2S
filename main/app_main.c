#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "graphics.h"
#include <math.h>
#include <sys/time.h>

static float _k = 0.0f;

void test(vec2*uv, vec3* out) {
    _k += 0.00001f;
    float iTime = _k;
    // v1
    float v1 = sin(uv->x*5.0 + iTime);
       
    // v2
    float v2 = sin(5.0*(uv->x*sin(iTime / 2.0) + uv->y*cos(iTime/3.0)) + iTime);
    
    // v3
    float cx = uv->x + sin(iTime / 5.0)*5.0;
    float cy = uv->y + sin(iTime / 3.0)*5.0;
    float v3 = sin(sqrt(100.0*(cx*cx + cy*cy)) + iTime);
    
    float vf = v1 + v2 + v3;
    float r  = cos(vf*M_PI);
    float g  = sin(vf*M_PI + 6.0*M_PI/3.0);
    float b  = cos(vf*M_PI + 4.0*M_PI/3.0);

    out->x = r;
    out->y = g;
    out->z = b;
}

void app_main() {
    graphics_init();
    graphics_module_start(test);

    //handle web requests, update buffer
    vTaskDelay(1000000000 / portTICK_PERIOD_MS);

    graphics_shutdown();
}
