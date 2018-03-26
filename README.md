# esp32_led_panel_i2c
The driver is mostly based on the experimental parallel i2c code provided by Espressif. https://esp32.com/viewtopic.php?t=3188. We added graphics ("shader") and network support.


# Setup

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