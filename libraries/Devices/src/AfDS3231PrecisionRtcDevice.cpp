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
// Created by Brian Smith 07/10/2024
//

#include "AfDS3231PrecisionRtcDevice.h"

#include "SerialBus.h"
#include "SerialBusDevice.h"

#include <cstdint>

using namespace Device;

constexpr uint8_t serialBusAddress = 0x68;

constexpr uint8_t secondsRegisterAddress = 0x00;

constexpr uint8_t timeValueDecimalMask = 0x70;
constexpr uint8_t convertBsd(uint8_t value, uint8_t decimalMask) {
  return ((decimalMask & value) >> 4) * 10 + (0x0f & value);
}

AfDS3231PrecisionRtcDevice::AfDS3231PrecisionRtcDevice(Core::SerialBus &bus)
    : Core::SerialBusDevice(bus, serialBusAddress) {}

void AfDS3231PrecisionRtcDevice::begin() {}

Core::RealTimeClockDevice::Time AfDS3231PrecisionRtcDevice::readTime() {
  TimeBuffer time;
  readRegisters(secondsRegisterAddress, &time.data[0], 3);
  return Time(convertBsd(time.time.seconds, timeValueDecimalMask), 
    convertBsd(time.time.minutes, timeValueDecimalMask), 0);
}