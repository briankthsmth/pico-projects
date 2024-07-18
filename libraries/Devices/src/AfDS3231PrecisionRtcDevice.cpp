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

#include "RealTimeClockDevice.h"
#include "SerialBus.h"
#include "SerialBusDevice.h"

#include <cstdint>

using namespace Core;
using namespace Device;

constexpr uint8_t serialBusAddress = 0x68;

constexpr uint8_t secondsRegisterAddress = 0x00;
constexpr uint8_t dayOfWeekRegisterAddress = 0x03;

constexpr uint8_t convertFromBcd(uint8_t value, uint8_t decimalMask) {
  return ((decimalMask & value) >> 4) * 10 + (0x0f & value);
}
constexpr uint8_t convertToBcd(uint8_t value, uint8_t decimalMask) {
  return (((value / 10) << 4) & decimalMask) + (0x0f & (value % 10));
}

constexpr uint8_t secondsDecimalMask      = 0x70;
constexpr uint8_t minutesDecimalMask      = secondsDecimalMask;
constexpr uint8_t hourDecimalMask         = 0x30;
constexpr uint8_t dayOfMonthDecimalMask   = 0x30;
constexpr uint8_t monthDecimalMask        = 0x10;
constexpr uint8_t yearDecimalMask         = 0xf0;

AfDS3231PrecisionRtcDevice::AfDS3231PrecisionRtcDevice(SerialBus &bus)
    : SerialBusDevice(bus, serialBusAddress) {}

void AfDS3231PrecisionRtcDevice::begin() {}

Time AfDS3231PrecisionRtcDevice::readTime() {
  TimeBuffer buffer;
  readRegisters(secondsRegisterAddress, &buffer.data[0], 3);
  
  return Time(
    convertFromBcd(buffer.time.seconds, secondsDecimalMask), 
    convertFromBcd(buffer.time.minutes, minutesDecimalMask), 
    convertFromBcd(buffer.time.hour, hourDecimalMask)
  );
}

Date AfDS3231PrecisionRtcDevice::readDate() {
  DateBuffer buffer;
  readRegisters(dayOfWeekRegisterAddress, &buffer.data[0], 4);
  
  return Date(
    buffer.date.dayOfWeek,
    convertFromBcd(buffer.date.dayOfMonth, dayOfMonthDecimalMask),
    convertFromBcd(buffer.date.month, monthDecimalMask),
    convertFromBcd(buffer.date.year, yearDecimalMask)
  );
}

ClockDatum AfDS3231PrecisionRtcDevice::read() {
  ClockDatumBuffer buffer;
  readRegisters(secondsRegisterAddress, &buffer.data[0], 7);
  
  return ClockDatum(
    Time(
      convertFromBcd(buffer.clockDatum.time.seconds, secondsDecimalMask), 
      convertFromBcd(buffer.clockDatum.time.minutes, minutesDecimalMask), 
      convertFromBcd(buffer.clockDatum.time.hour, hourDecimalMask)      
    ),
    Date(
      buffer.clockDatum.date.dayOfWeek,
      convertFromBcd(buffer.clockDatum.date.dayOfMonth, dayOfMonthDecimalMask),
      convertFromBcd(buffer.clockDatum.date.month, monthDecimalMask),
      convertFromBcd(buffer.clockDatum.date.year, yearDecimalMask)
    )
  );
}

void AfDS3231PrecisionRtcDevice::write(ClockDatum clockDatum) {
  ClockDatumBuffer buffer;
  buffer.clockDatum.time.seconds = convertToBcd(clockDatum.time.seconds, secondsDecimalMask);
  buffer.clockDatum.time.minutes = convertToBcd(clockDatum.time.minutes, minutesDecimalMask);
  buffer.clockDatum.time.hour = convertToBcd(clockDatum.time.hour, hourDecimalMask);
  buffer.clockDatum.date.dayOfWeek = clockDatum.date.dayOfWeek;
  buffer.clockDatum.date.dayOfMonth = convertToBcd(clockDatum.date.dayOfMonth, dayOfMonthDecimalMask);
  buffer.clockDatum.date.month = convertToBcd(clockDatum.date.month, monthDecimalMask);
  buffer.clockDatum.date.year = convertToBcd(clockDatum.date.year, yearDecimalMask);
  
  writeRegisters(secondsRegisterAddress, &buffer.data[0], 7);
}

