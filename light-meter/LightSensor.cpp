// BSD 3-Clause License
//
// Copyright (c) 2024, Brian Keith Smith
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
//
// Created by Brian Smith 05/20/2024
//

#include "LightSensor.h"

#include <cstdint>

#include "hardware/i2c.h"

constexpr uint8_t address = 0x10;

constexpr uint8_t als_config_register     = 0x00;
constexpr uint8_t ambient_light_register  = 0x04;
constexpr uint8_t white_register          = 0x05;

using namespace LightMeter;

void LightSensor::init() {
  uint8_t configuration[3] = {als_config_register, 0x00, 0x13};
  i2c_write_blocking(i2c_default, address, &als_config_register, 3, false);

  sleep_ms(2);
}

float LightSensor::readAmbientLight() {
  uint8_t read_buf[2] = {0, 0};
  i2c_write_blocking(i2c_default, address, &ambient_light_register, 1, true);
  i2c_read_blocking(i2c_default, address, &read_buf[0], 2, false);

  uint16_t value = (static_cast<uint16_t>(read_buf[1]) << 8) +
                   static_cast<uint16_t>(read_buf[0]);

  float lux = value * 2.1504;

  return lux;
}