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
// Created by Brian Smith 07/23/2024
//

#pragma once

#include "Device.h"
#include "DisplayRenderable.h"
#include "SerialBus.h"
#include "SerialBusDevice.h"
#include <cstdint>

namespace Device {

class Af128x64FeatherMonoDisplayDevice final : 
  public Core::DisplayRenderable, Core::Device,
  private Core::SerialBusDevice 
{
public:
  Af128x64FeatherMonoDisplayDevice(Core::SerialBus& bus);
  ~Af128x64FeatherMonoDisplayDevice() = default;
  
  void init() override;
  
  void render(uint8_t *data, Core::DisplayRenderable::RenderArea area) override;
  void clear() override;
  Properties getProperties() const override;
  
private:
  enum DataType {command, ram};
  void writeCommandList(uint8_t*, int);
  void writeInPage(uint8_t, uint8_t, uint8_t*, int);
  void write(uint8_t*, int, DataType);
}; // class Af128x64FeatherMonoDisplayDevice

} // namespace Device