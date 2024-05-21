# Light Controller

## Hardware
- [Sparkfun Pro Micro - RP2040](https://www.sparkfun.com/products/18288)
- [Sparkfun Real Time Clock Module - RV-8803](https://www.sparkfun.com/products/16281)
- [LED - RGB Diffused Common Cathode](https://www.sparkfun.com/products/9264)

## Software

When generating makefiles with cmake use the following to specify the Sparkfun micro.

```cmake .. -DPICO_BOARD=sparkfun_promicro```