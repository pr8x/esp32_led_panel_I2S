# ESP32 LED PANEL
The driver is mostly based on the experimental parallel I2S code provided by Espressif. https://esp32.com/viewtopic.php?t=3188. We added graphics ("shader") and network support.

# Features

- Uses the I2S periphal
- Low cpu consumption
- Modules (shader)
- Double-buffering
- Loading/sampling .gif images from file system
- Web interface

![Alt Text](demo_march_2018.gif)

# Wiring

The RGB panel has two 16 pin connector on the back. Make sure to use the one labeled "DATA_IN" or "JIN". Most panels use the following pinout:

|  L  |  R  |
|-----|-----|
| R1  | G1  |
| B1  | GND |
| R2  | G2  |
| B2  | GND |
| A   | B   |
| C   | D   |
| CLK | LAT |
| OE  | GND |

You can simply use jumper wires to connect them to ESP32 GPIO pins. We built a small shield for that purpose, but we're eventually planning to print a custom PCB.

| Panel | ESP32  |
|-------|--------|
| R1    | GPIO2  |
| G1    | GPIO15 |
| B1    | GPIO4  |
| R2    | GPIO16 |
| G2    | GPIO27 |
| B2    | GPIO17 |
| A     | GPIO5  |
| B     | GPIO18 |
| C     | GPIO19 |
| D     | GPIO21 |
| LAT   | GPIO26 |
| OE    | GPIO25 |
| CLK   | GPIO22 |


# Install

Follow the [official instructions](https://esp-idf.readthedocs.io/en/v2.0/linux-setup.html) to setup the ESP32 toolchain.

Clone this repository and run `make flash && make monitor` to flash the firmware and monitor serial output. You might need to adjust the upload port in the toolchain settings. Run `make menuconfig` to do so.

In order to display custom (64x32px) `.gif` files you need to upload them into the SPIFFS partition of the ESP. Install [mkspiffs](https://github.com/igrr/mkspiffs) and copy your `.gif` files into /data/.

`mkspiff -c data -b 4096 -p 256 -s 2097152 spiffs.bin`

Finally, upload the image using `esptool.py`. Make sure to adjust the `--port` option accordingly.

`esptool --chip esp32 --port /dev/tty.SLAB_USBtoUART --baud 115200 write_flash 0x180000 spiffs.bin`

You can also run `upload_spiffs.sh`, which automates this upload process.
