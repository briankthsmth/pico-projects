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