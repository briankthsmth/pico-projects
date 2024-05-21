# Light Meter

## Hardware
- [Sparkfun Pro Micro - RP2040](https://www.sparkfun.com/products/18288)
- [Sparkfun Micro OLED](https://www.sparkfun.com/products/retired/14532) (retired)
- [Adafruit VEML 7700](https://www.adafruit.com/product/4162)

## Software

When generating makefiles with cmake use the following to specify the Sparkfun micro.

```cmake .. -DPICO_BOARD=sparkfun_promicro```