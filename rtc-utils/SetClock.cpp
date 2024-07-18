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
// Created by Brian Smith 07/12/2024
//

#include <cstdint>
#include <cstdio>

#include "pico/stdlib.h"

#include "AfDS3231PrecisionRtcDevice.h"
#include "SerialBus.h"
#include "RealTimeClockDevice.h"

uint8_t promptForInput(const char* message) {
  unsigned int input;

  printf("%s", message);
  scanf("%u", &input);
  auto value = static_cast<uint8_t>(input);
  printf("%u\n", value);
  
  return value;
}

int main() {
  stdio_init_all();
  
  Core::ClockDatum clockReading;
  
  clockReading.date.dayOfWeek = promptForInput("Day of week? (1 = Sunday, ..., 7 = Saturday) ");
  clockReading.date.month = promptForInput("Month? (1...12) ");
  clockReading.date.dayOfMonth = promptForInput("Day of month? (1...31) ");
  clockReading.date.year = promptForInput("Year? (0...99)");
  clockReading.time.hour = promptForInput("Hour? (0...23)");
  clockReading.time.minutes = promptForInput("Minutes? (0...59)");
  clockReading.time.seconds = promptForInput("Seconds? (0...59)");
  
  std::printf("You entered: %s", clockReading.toString());
  
  Core::SerialBus serialBus;
  Device::AfDS3231PrecisionRtcDevice rtc(serialBus);
  rtc.write(clockReading);
  
  auto setClockReading = rtc.read();
  printf("RTC set to: %s", setClockReading.toString());
  
  while (1) {}
  
  return 0;
}