// Simple program to print the date and time from a real time clock.
// 
// Created by Brian Smith 05/09/2023

#include <iostream>
#include "RealClock.hpp"
#include "pico/stdlib.h"

using namespace Peripheral;

int main() {
	stdio_init_all();
	
	RealClock clock;
	char string_buffer[256];
	
	while(1) {
		datetime_t dt = clock.get_datetime();
		datetime_to_str(&string_buffer[0], 256, &dt);
		std::cout << reinterpret_cast<char *>(&string_buffer[0]) << "\r" << std::flush;
		 
		sleep_ms(500);
	}
	
	return 0;
}