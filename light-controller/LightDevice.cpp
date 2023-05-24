// BSD 3-Clause License
//
// Copyright (c) 2023, Brian Keith Smith
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
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
