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