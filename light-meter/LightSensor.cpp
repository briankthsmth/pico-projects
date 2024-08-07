// BSD 3-Clause License
//
// Copyright (c) 2024, Brian Keith Smith
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//
//
// Created by Brian Smith 05/20/2024
//

#include "LightSensor.h"
#include "AlsConfigRegister.h"

#include "hardware/i2c.h"
#include "pico/time.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>

constexpr uint8_t address = 0x10;

//
// LUX correction constants
//
const float c4 = 6.0135E-13;
const float c3 = -9.3924E-9;
const float c2 = 8.1488E-5;
const float c1 = 1.0023;

//
// Command codes
//
constexpr uint8_t als_config_command_code = 0x00;
constexpr uint8_t ambient_light_command_code = 0x04;
constexpr uint8_t white_channel_command_code = 0x05;

//
// ALS Constants
//
constexpr uint16_t als_count_limit = 10000;
// LUX multiplier. Ordered by integration time then gain.
const float lux_multipliers[6][4] = {
  {2.1504, 1.0752, 0.2688, 0.1344},
  {1.0752, 0.5376, 0.1344, 0.0672},
  {0.5376, 0.2688, 0.0672, 0.0336},
  {0.2688, 0.1344, 0.0336, 0.0168},
  {0.1344, 0.0672, 0.0168, 0.0084}, 
  {0.0672, 0.0336, 0.0084, 0.0042}
};
const uint32_t integration_times[] = {25, 50, 100, 200, 400, 800}; //ms
const float gain_values[] = {0.125, 0.25, 1, 2};

using namespace LightMeter;

//
// Public Interface
//
void LightSensor::init() {
  AlsConfigRegister config_register;
  config_register.setting.power = AlsConfigRegister::on;
  writeConfigRegister(config_register);
  
  sleep_ms(3);
}

void LightSensor::read() {
  AlsConfigRegister config_register = readConfigRegister();

  uint16_t counts = readAmbientLightRegister();

  while (counts < 100 && config_register.getIntegrationTime() != AlsConfigRegister::ms_800) 
  {       
    shutdown(config_register);
    
    if (config_register.getGain() != AlsConfigRegister::high) {
      config_register.increaseGain();
      writeConfigRegister(config_register);
    } else if (config_register.getIntegrationTime() != AlsConfigRegister::ms_800) {
      config_register.increaseIntegrationTime();
      writeConfigRegister(config_register);
    }
    
    powerOn(config_register);
    counts = readAmbientLightRegister();
  }

  while (counts > als_count_limit &&
         config_register.getIntegrationTime() != AlsConfigRegister::ms_25) 
  {
    shutdown(config_register);
    config_register.decreaseIntegrationTime();
    writeConfigRegister(config_register);
    powerOn(config_register);
    
    counts = readAmbientLightRegister();
  }
  
  gainWhenRead = config_register.getGain();
  integrationTimeWhenRead = config_register.getIntegrationTime();
  setAmbientLightLux(readAmbientLightRegister());
  setWhiteChannel(readWhiteChannelRegister());
  
  // restore the gain and integration times to 1/8 and 100 ms.
  shutdown(config_register);
  config_register.setGain(AlsConfigRegister::low);
  config_register.setIntegrationTime(AlsConfigRegister::ms_100);
  writeConfigRegister(config_register);
  powerOn(config_register);
}

//
// Private Interface
//

///
/// \brief Set the ambient light lux value with the channel count.
///
/// \details Calculates the lux value by multiplying adjusted for gain and integration
///  time. If the value is over 1000 lux the value is adjusted by a fourth order
///  polynomial for non-linearity in the sensor. Above 10,000 counts the sensor is 
///  consider very non-linear, so the value is clamped to this value.
///
/// \params counts The ambient light sensor counts.
///
void LightSensor::setAmbientLightLux(uint16_t counts) {
  // Based on the application notes the sensor is not very accurate above 10,000 counts.
  // So, limit the counts to this if increasing integration time failed.
  counts = std::min(als_count_limit, counts);
  
  int time_index =  integrationTimeWhenRead;
  int gain_index = gainWhenRead;
  float multiplier = lux_multipliers[time_index][gain_index];
  float lux = counts * multiplier;
  if (lux > 1000.0 && getGainWhenRead() < AlsConfigRegister::medium_high) {
    lux = (((c4 * lux + c3) * lux + c2) * lux + c1) * lux;
  }
  ambientLightLux = lux;
}

///
/// \brief Set the white channel value.
///
/// \details Set the white channel value from the counts normalizing it for the gain and 
/// integration time when the register was read. The value is in counts/ms.
///
/// \param count The white channel count.
/// 
void LightSensor::setWhiteChannel(uint16_t count) {
  float time = static_cast<float>(integration_times[getIntegrationTimeWhenRead()]);
  float gain = gain_values[getGainWhenRead()];
  whiteChannel = static_cast<float>(count) / (gain * time);
}

void LightSensor::powerOn(AlsConfigRegister& config_register) {
  config_register.setting.power = AlsConfigRegister::on;
  writeConfigRegister(config_register);
  uint32_t integration_time = integration_times[config_register.getIntegrationTime()];
  uint32_t delay_time = integration_time + (integration_time * 18)/100;
  sleep_ms(delay_time);
}

void LightSensor::shutdown(AlsConfigRegister& config_register) {
  config_register.setting.power = AlsConfigRegister::off;
  writeConfigRegister(config_register);
}

AlsConfigRegister LightSensor::readConfigRegister() {
  Register reg = readRegister(als_config_command_code);
  return AlsConfigRegister(reg.value);
}

void LightSensor::writeConfigRegister(AlsConfigRegister config_register) {
  writeRegister(als_config_command_code, Register(config_register.value));
}

uint16_t LightSensor::readAmbientLightRegister() {
  Register reg = readRegister(ambient_light_command_code);
  return reg.value;
}

uint16_t LightSensor::readWhiteChannelRegister() {
  Register reg = readRegister(white_channel_command_code);
  return reg.value;
}

LightSensor::Register LightSensor::readRegister(CommandCode command_code) {
  uint8_t buffer[2];
  i2c_write_blocking(i2c_default, address, &command_code, 1, true);
  i2c_read_blocking(i2c_default, address, &buffer[0], 2, false);
  
  Register reg;
  reg.data_byte.lsb = buffer[0];
  reg.data_byte.msb = buffer[1];
  return reg;
}

void LightSensor::writeRegister(CommandCode command_code, Register reg) {
  uint8_t buffer[] = {command_code, reg.data_byte.lsb, reg.data_byte.msb};
  i2c_write_blocking(i2c_default, address, &buffer[0], 3, false);
}