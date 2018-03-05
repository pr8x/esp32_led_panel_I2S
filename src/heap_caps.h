#ifndef HEAP_CAPS_H
#define HEAP_CAPS_H

//PlatformIO currently does not support esp-idf 3.0
//#include "esp_heap_caps.h"

#include "esp_heap_alloc_caps.h"
#define heap_caps_malloc pvPortMallocCaps

#endif