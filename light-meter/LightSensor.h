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

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <cstdint>

#include "AlsConfigRegister.h"

namespace LightMeter {

typedef uint8_t CommandCode;

class LightSensor {
public:
  LightSensor() = default;
  ~LightSensor() = default;
  
  void init();
  /*
   * Read the ambient light register and calculate the lux. Plus, read the white
   * channel register.
   */
  void read();

  float getAmbientLightLux() { return ambientLightLux; }
  float getWhiteChannel() { return whiteChannel; }
  AlsConfigRegister::Gain getGainWhenRead() { return gainWhenRead; }
  AlsConfigRegister::IntegrationTime getIntegrationTimeWhenRead() { 
    return integrationTimeWhenRead; 
  }
  
private:
  struct Register {
    union {
      uint16_t value;
      struct {
        uint8_t lsb : 8;
        uint8_t msb : 8;
      } data_byte;
    };
    Register() : value(0) {}
    Register(uint16_t value) : value(value) {}
  };
  
  float ambientLightLux = 0;
  float whiteChannel = 0;
  AlsConfigRegister::Gain gainWhenRead;
  AlsConfigRegister::IntegrationTime integrationTimeWhenRead;
  
  
  void setAmbientLightLux(uint16_t);
  void setWhiteChannel(uint16_t);

  void powerOn(AlsConfigRegister&);
  void shutdown(AlsConfigRegister&);
  
  AlsConfigRegister readConfigRegister();
  void writeConfigRegister(AlsConfigRegister);
  
  uint16_t readAmbientLightRegister();
  uint16_t readWhiteChannelRegister();
  
  Register readRegister(CommandCode);
  void writeRegister(CommandCode, Register);
};

}; // namespace LightMeter

#endif // LIGHTSENSOR_H