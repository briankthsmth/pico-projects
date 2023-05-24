// Program to set the date and time from a real time clock.
// 
// Created by Brian Smith 05/09/2023

#include <iostream>
#include "RealClock.hpp"
#include "pico/stdlib.h"

using namespace Peripheral;
using namespace std;

constexpr int16_t base_year = 2000;

int stream_input_number(string prompt, int min, int max) {
	int value;
	do {
		while (cout << prompt && !(cin >> value)) {
			cin.clear();
			char character;
			cin.get(character);
			cout << "invalid character" << endl;
		}
		cout << value << endl;
	} while ( value < min || value > max);
	return value;
}

int main() {
	stdio_init_all();
	
	int month = stream_input_number("Enter month (1 -> 12): ", 1, 12);		
	int day = stream_input_number("Enter day (1 -> 31): ", 1, 31);	
	int year = stream_input_number("Enter year (0 -> 99): ", 0, 99);	
	int dotw = stream_input_number("Enter day of the week (0 -> 6): ", 0, 6);	
	int hour = stream_input_number("Enter the hour (0 -> 23): ", 0, 23);
	int min = stream_input_number("Enter the minutes (0 -> 59): ", 0, 59);
	
	int16_t full_year = base_year + static_cast<int16_t>(year);
	
	datetime_t datetime = {
		.year = full_year,
		.month = static_cast<int8_t>(month),
		.day = static_cast<int8_t>(day),
		.dotw = static_cast<int8_t>(dotw),
		.hour = static_cast<int8_t>(hour),
		.min = static_cast<int8_t>(min),
		.sec = 0
	};
	
	RealClock clock;
	
	clock.set_datetime(datetime);
	
	datetime_t set_datetime = clock.get_datetime();
	char datetime_string_buffer[256];
	datetime_to_str(&datetime_string_buffer[0], sizeof(datetime_string_buffer), &set_datetime);
	cout << "The clock is set to " << string(&datetime_string_buffer[0]) << endl;
	
	return 0;
}