#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef void (*module_func_t)(vec2*, vec3*);

void graphics_init();
void graphics_shutdown();
void graphics_module_start(module_func_t);

#endif