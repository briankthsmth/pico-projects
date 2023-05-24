// Created by Brian Smith 5/3/2023

#include "pico/stdlib.h"

#ifndef LightDevice_hpp
#define LightDevice_hpp

namespace Peripheral {
	// Controls a RGB LED using PWM outputs.
	class LightDevice {
	public:
		LightDevice(const uint red_gpio_pin, const uint green_gpio_pin, 
			const uint blue_gpio_pin);
		~LightDevice() = default;
		
		// Colors values are from 0 to 255.
		void set_color(const uint8_t red, const uint8_t green, const uint8_t blue);
		
	private:
		uint red_gpio_pin;
		uint green_gpio_pin;
		uint blue_gpio_pin;
		
		void configure_gpio_pin(uint gpio_pin);
		
		LightDevice(const LightDevice&) = delete;
		LightDevice& operator=(const LightDevice&) = delete;		
	}; // class LightDevice

}; // namespace Peripheral
#endif // LightDevice_hpp