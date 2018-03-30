#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

//ESP32 only supports a forensic GDB, so causing an exception is basically a breakpoint
#define GDB_CRASH volatile uint32_t _crash = *(uint32_t*)0;