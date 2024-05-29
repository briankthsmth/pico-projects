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
// Created by Brian Smith 05/22/2024
//

#ifndef ALSCONFIGREGISTER_H
#define ALSCONFIGREGISTER_H

#include <cstdint>

namespace LightMeter {

struct AlsConfigRegister {
  union {
    uint16_t value;
    struct {
      uint8_t lsb : 8;
      uint8_t msb : 8;
    } data_byte;
    struct {
      uint8_t power                   : 1;
      uint8_t interupt_enable         : 1;
      uint8_t                         : 2;
      uint8_t persistance_protect     : 2;
      uint8_t integration_time_lower  : 2; // Split into 2 bits as it straddles the LSB and MSB
      uint8_t integration_time_upper  : 2;      
      uint8_t                         : 1;
      uint8_t gain                    : 2;
    } setting;
  };

  enum Power : uint8_t { on, off };
  enum InterruptEnable : uint8_t { disable, enable };
  enum PersistanceProtect : uint8_t { one, two, four, eight };
  enum IntegrationTime : int {
    ms_25,
    ms_50,
    ms_100,
    ms_200,
    ms_400,
    ms_800,
  };
  
  enum Gain : int {
    low,         // 1/8
    medium_low,  // 1/4
    medium_high, // 1
    high         // 2
  };
  
  
  AlsConfigRegister();
  AlsConfigRegister(uint16_t value) : value(value) {}
  
  void setGain(Gain);
  Gain getGain();
  void increaseGain();
  void decreaseGain();

  void setIntegrationTime(IntegrationTime);
  IntegrationTime getIntegrationTime();  
  void increaseIntegrationTime();
  void decreaseIntegrationTime();
};

}; // namespace LightMeter

#endif //  ALSCONFIGREGISTER_H