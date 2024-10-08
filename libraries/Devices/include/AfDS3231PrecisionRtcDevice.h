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

#pragma once

#include "Clock.h"
#include "Device.h"
#include "RealTimeClockDevice.h"
#include "SerialBus.h"
#include "SerialBusDevice.h"

namespace Device {

class AfDS3231PrecisionRtcDevice final : 
  public Core::RealTimeClockDevice, Core::Device,
  private Core::SerialBusDevice 
{
public:
  AfDS3231PrecisionRtcDevice(Core::SerialBus &);
  ~AfDS3231PrecisionRtcDevice() = default;

  void init() override;

  Core::Time readTime() override;
  Core::Date readDate() override;
  Core::ClockDatum read() override;
  
  void write(Core::ClockDatum clockDatum) override;

private:
  struct TimeBuffer {
    union {
      uint8_t data[3];
      Core::Time time;
    };
  };

  struct DateBuffer {
    union {
      uint8_t data[4];
      Core::Date date;
    };
  };

  struct ClockDatumBuffer {
    union {
      uint8_t data[7];
      Core::ClockDatum clockDatum;
    };
  };

}; // class AfDS3231PrecisionRtcDevice

}; // namespace Device