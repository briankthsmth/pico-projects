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
// Created by Brian Smith 07/03/2024
//

#include "PowerDevice.h"

#include "pico/stdlib.h"
#include "pico/time.h"

#include <cstdio>

#include "Af128x64FeatherMonoDisplayDevice.h"
#include "AfDS3231PrecisionRtcDevice.h"
#include "AfPowerRelayDevice.h"
#include "ControlConfiguration.h"
#include "SerialBus.h"
#include "TimeScheduler.h"

int main() {

  //
  // Setup
  //
  stdio_init_all();
  
  Core::SerialBus serialBus;
  
  Device::Af128x64FeatherMonoDisplayDevice displayDevice(serialBus);
  displayDevice.init();
  
  Device::AfDS3231PrecisionRtcDevice timeDevice(serialBus); 
  timeDevice.init();
  
  Device::AfPowerRelayDevice powerDevice(Device::RelayControlGpio::gpio10);
  powerDevice.init();
  
  Core::ControlConfiguration configuration;
  configuration.startTime.hour = 6; 
  configuration.startTime.minutes = 30;
  configuration.startTime.seconds = 0;
  configuration.endTime.hour = 20;
  configuration.endTime.minutes = 0;
  configuration.endTime.seconds = 0;
  
  Core::TimeScheduler scheduler(powerDevice, timeDevice, configuration);
  
  
loop:
  auto time = timeDevice.readTime();
  scheduler.update();
  
  sleep_ms(1000); // sleep for 1 seconds
  goto loop;
  
  return 0;
}