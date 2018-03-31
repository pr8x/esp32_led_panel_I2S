#!/bin/bash

mkspiff -c data -b 4096 -p 256 -s 2097152 spiffs.bin
esptool --chip esp32 --port /dev/tty.SLAB_USBtoUART --baud 115200 write_flash 0x180000 spiffs.bin