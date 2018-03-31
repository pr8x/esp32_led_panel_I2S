#ifndef NETWORK_H
#define NETWORK_H

#define NETWORK_SSID "522"
#define NETWORK_PSK ""

#include "common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "http_server.h"

typedef void (*request_callback_t)(http_context_t* ctx);

void network_init();
void network_set_callback(request_callback_t);
void network_shutdown();

#endif