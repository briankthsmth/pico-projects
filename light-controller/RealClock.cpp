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
// Create by Brian Smith 4/27/2023

#include "RealClock.hpp"

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <iostream>


using namespace Peripheral;


/*
*	Constants
*/
const uint8_t address = 0x32; // address of RV-8803 RTC
const uint8_t RealClock::Register::seconds = 0x00;
constexpr int16_t base_year = 2000;


/*
*	Utility Functions
*/
// Functions to convert valaue to/from BCD which the RV-8803 uses.
constexpr uint8_t to_bcd(uint8_t value) {
	return ((value / 10) << 4) + (value % 10);
}

constexpr uint8_t bcd_to_u8(uint8_t bcd_value) { 
	return ((bcd_value >> 4) * 10) + (bcd_value & 0x0F); 
}
constexpr int8_t bcd_to_i8(uint8_t bcd_value) { 
	// safe to convert this to an unsigned char as the values range from 0 to 7
	return static_cast<int8_t>(bcd_to_u8(bcd_value)); 
}
constexpr int16_t bcd_to_i16(uint8_t bcd_value) {
	return static_cast<int16_t>(bcd_to_u8(bcd_value));
}

// Functions to handle converting day of the week values.
// The RV-8803 uses set bits to indicate day of the week. 
int8_t convert_bitfield_dotw(uint8_t value) {
	int8_t dow = 0;
	
	while (value > 1) { 
		value = value >> 1; 
		dow++;
	}
	return dow;
}

/*
*	RealClock class API
*/
RealClock::RealClock() {
	i2c_init(i2c_default, 100 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);	
}

void RealClock::set_datetime(const datetime_t datetime) {
	uint8_t date_registers[8];
	date_registers[0] = RealClock::Register::seconds; // set starting register
	date_registers[1] = to_bcd(datetime.sec);
	date_registers[2] = to_bcd(datetime.min);
	date_registers[3] = to_bcd(datetime.hour);
	date_registers[4] = 1 << datetime.dotw;
	date_registers[5] = to_bcd(datetime.day);
	date_registers[6] = to_bcd(datetime.month);
	date_registers[7] = to_bcd(datetime.year - base_year);
	
	i2c_write_blocking(i2c_default, address, &date_registers[0], sizeof(date_registers), false);
}

datetime_t RealClock::get_datetime() {
	uint8_t bcd_values[7];
	
	i2c_write_blocking(i2c_default, address, &RealClock::Register::seconds, 1, true);
	i2c_read_blocking(i2c_default, address, &bcd_values[0], 7, false);
	 
	int16_t year = bcd_to_i16(bcd_values[6]) + base_year;
	
	datetime_t date_time = {
	 .year = year,
	 .month = bcd_to_i8(bcd_values[5]),
	 .day = bcd_to_i8(bcd_values[4]),
	 .dotw = convert_bitfield_dotw(bcd_values[3]),
	 .hour = bcd_to_i8(bcd_values[2]),
	 .min = bcd_to_i8(bcd_values[1]),
	 .sec = bcd_to_i8(bcd_values[0])
	};
	
	return date_time;
}
