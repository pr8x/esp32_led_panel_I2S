#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <math.h>
#include <stdbool.h>

#include "filesystem.h"
#include "gifdec.h"
#include "common.h"

typedef struct {
    char file[20];
    bool loop;
    int anim_speed;

    unsigned char* _buffer;
    gd_GIF* _gif;
} sampler_t;

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

inline float dot2(vec2 a, vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

inline float dot3(vec3 a, vec3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline float length2(vec2 a) {
    return sqrt(dot2(a,a));
}

inline float length3(vec3 a) {
    return sqrt(dot3(a,a));
}

inline void sample(sampler_t* sampler, vec2 in, vec3* out) {
    int x = (int)(in.x * 64);
    int y = (int)(in.y * 32);
    unsigned char* p = sampler->_buffer + ((x + y * 64) * 3);
    out->x = p[0] / 255.0f;
    out->y = p[1] / 255.0f;
    out->z = p[2] / 255.0f;
}

typedef void (*module_func_t)(vec2*, vec4*, sampler_t*);

typedef struct {
    sampler_t* sampler;
    module_func_t fn;
} module_t;

void graphics_init();
void graphics_shutdown();
void graphics_run(module_t*);
void graphics_stop();

#endif