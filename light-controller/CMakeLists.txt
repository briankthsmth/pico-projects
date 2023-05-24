cmake_minimum_required(VERSION 3.26)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(light_controller C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(light_controller 
	light_controller.cpp
	RealClock.cpp
	LightDevice.cpp
)
target_link_libraries(
	light_controller 
	pico_stdlib 
	hardware_i2c 
	hardware_pwm
)
pico_add_extra_outputs(light_controller)

add_executable(print_clock
	print_clock.cpp
	RealClock.cpp
)
target_link_libraries(
	print_clock
	pico_stdlib
	hardware_i2c
)
pico_add_extra_outputs(print_clock)

add_executable(set_clock
	set_clock.cpp
	RealClock.cpp
)
target_link_libraries(
	set_clock
	pico_stdlib
	hardware_i2c
)
pico_add_extra_outputs(set_clock)