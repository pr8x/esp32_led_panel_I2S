//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "i2s_parallel.h"
#include "common.h"

//My display has each row swapped with its neighbour (so the rows are 2-1-4-3-6-5-8-7-...). If your display
//is more sane, uncomment this to get a good image.
#define DISPLAY_ROWS_SWAPPED 1

//This is the bit depth, per RGB subpixel, of the data that is sent to the display.
//The effective bit depth (in computer pixel terms) is less because of the PWM correction. With
//a bitplane count of 7, you should be able to reproduce an 16-bit image more or less faithfully, though.
#define BITPLANE_CNT 7

//64*32 RGB leds, 2 pixels per 16-bit value...
#define BITPLANE_SZ (64*32/2)

//Change to set the global brightness of the display, range 1-63
//Warning when set too high: Do not look into LEDs with remaining eye.
#define BRIGHTNESS 48


//Upper half RGB
#define BIT_R1 (1<<0)   //connected to GPIO2 here
#define BIT_G1 (1<<1)   //connected to GPIO15 here
#define BIT_B1 (1<<2)   //connected to GPIO4 here

//Lower half RGB
#define BIT_R2 (1<<3)   //connected to GPIO16 here
#define BIT_G2 (1<<4)   //connected to GPIO27 here
#define BIT_B2 (1<<5)   //connected to GPIO17 here

#define BIT_A (1<<8)    //connected to GPIO5 here
#define BIT_B (1<<9)    //connected to GPIO18 here
#define BIT_C (1<<10)   //connected to GPIO19 here
#define BIT_D (1<<11)   //connected to GPIO21 here
#define BIT_LAT (1<<12) //connected to GPIO26 here
#define BIT_OE (1<<13)  //connected to GPIO25 here

#define REFRESH_RATE 16 
#define BUFFER_SIZE 64 * 32 * 3

void driver_init();
void driver_run();
void driver_shutdown();

void driver_set_buffer(unsigned char*);

#endif