// Create by Brian Smith 4/26/2023

#include <iostream>
#include "RealClock.hpp"
#include "LightDevice.hpp"
#include "pico/stdlib.h"

using namespace Peripheral;

const uint red_gpio_pin = 2;
const uint green_gpio_pin = 3;
const uint blue_gpio_pin = 4;

int main() {
	stdio_init_all();
	
	RealClock clock;
	
	LightDevice light_device = LightDevice(red_gpio_pin, green_gpio_pin, blue_gpio_pin);
	
	while(1) {
		datetime_t dt = clock.get_datetime();
		// This just blinks the light at full intensity every 30 seconds. It is 
		// simple way to see that LED is working.
		if (dt.sec < 30) {
			light_device.set_color(255, 255, 255);
		} else {
			light_device.set_color(0, 0, 0);
		}
		sleep_ms(500);
	}
	return 0;
}