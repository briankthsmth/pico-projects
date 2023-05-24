// Created by Brian Smith 5/3/2023

#include <iostream>

#include "LightDevice.hpp"
#include "hardware/pwm.h"

using namespace Peripheral;

LightDevice::LightDevice(const uint red_gpio_pin, const uint green_gpio_pin, const uint blue_gpio_pin) :
	red_gpio_pin(red_gpio_pin), 
	green_gpio_pin(green_gpio_pin), 
	blue_gpio_pin(blue_gpio_pin) 
{
	gpio_set_function(red_gpio_pin, GPIO_FUNC_PWM);
	gpio_set_function(green_gpio_pin, GPIO_FUNC_PWM);
	gpio_set_function(blue_gpio_pin, GPIO_FUNC_PWM);

	configure_gpio_pin(red_gpio_pin);
	configure_gpio_pin(green_gpio_pin);	
	configure_gpio_pin(blue_gpio_pin);
}

void LightDevice::set_color(const uint8_t red, const uint8_t green, const uint8_t blue) {
	// Color values are squared as the pwm API allows for 16 bit values.
	pwm_set_gpio_level(red_gpio_pin, red * red);
	pwm_set_gpio_level(green_gpio_pin, green * green);
	pwm_set_gpio_level(blue_gpio_pin, blue * blue);
}

void LightDevice::configure_gpio_pin(uint gpio_pin) {
	uint slice_num = pwm_gpio_to_slice_num(gpio_pin);
	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);
	pwm_init(slice_num, &config, true);
}
