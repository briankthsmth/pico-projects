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
// Created by Brian Smith 05/22/2024
//

#include "AlsConfigRegister.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>

constexpr uint8_t default_gain = 0x02;

const uint8_t gain_values[] = {0x02, 0x03, 0x00, 0x01};
constexpr int gain_values_count = 4;
const uint8_t integration_time_values[] = {0x0c, 0x08, 0x00, 0x01, 0x02, 0x03};
constexpr int integration_time_values_count = 6;

using namespace LightMeter;

AlsConfigRegister::AlsConfigRegister() : value(0x0001) {
  setting.gain = default_gain;
}

void AlsConfigRegister::setGain(Gain gain) {
  auto value = gain_values[gain];
  setting.gain = value;
}

AlsConfigRegister::Gain AlsConfigRegister::getGain() {
  auto value = setting.gain;
  
  Gain gain = low;
  int index = 0;
  for (int index = 0; index < gain_values_count; ++index) {
    if (gain_values[index] == value) {
      gain = Gain(index);
    }
  }

  return gain;
}

void AlsConfigRegister::increaseGain() {
  auto gain = getGain();
  int gain_index = static_cast<int>(gain) + 1;
  gain_index = std::min(gain_index, static_cast<int>(gain_values_count - 1));
  setGain(Gain(gain_index));
}

void AlsConfigRegister::decreaseGain() {
  auto gain = getGain();
  int gain_index = static_cast<int>(gain) - 1;
  gain_index = std::max(gain_index, 0);
  setGain(Gain(gain_index));
}

void AlsConfigRegister::setIntegrationTime(IntegrationTime time) {
  auto value = integration_time_values[time];
  setting.integration_time_lower = value & 0x03;
  setting.integration_time_upper = value >> 2;
}

AlsConfigRegister::IntegrationTime AlsConfigRegister::getIntegrationTime() {
  uint8_t value = (setting.integration_time_lower | (setting.integration_time_upper << 2));
  
  IntegrationTime time = ms_100;
  for (int index = 0; index < integration_time_values_count; ++index) {
    if (integration_time_values[index] == value) {
      time = IntegrationTime(index);
    }
  }
  
  return time;
}

void AlsConfigRegister::increaseIntegrationTime() {
  auto time = getIntegrationTime();
  int time_index = static_cast<int>(time) + 1;
  time_index = std::min(time_index, static_cast<int>(integration_time_values_count - 1));
  setIntegrationTime(IntegrationTime(time_index));
}

void AlsConfigRegister::decreaseIntegrationTime() {
  auto time = getIntegrationTime();
  int time_index = static_cast<int>(time) - 1;
  time_index = std::max(time_index, 0);
  setIntegrationTime(IntegrationTime(time_index));
}