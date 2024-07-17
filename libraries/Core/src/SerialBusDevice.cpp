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
// Created by Brian Smith 07/10/2024
//

#include "SerialBusDevice.h"

#include "SerialBus.h"
#include <cstdint>
#include <cstring>

using namespace Core;

SerialBusDevice::~SerialBusDevice() {}

void SerialBusDevice::writeRegister(uint8_t address, uint8_t data) {
  writeRegisters(address, &data, 1);
}

uint8_t SerialBusDevice::readRegister(uint8_t address) {
  uint8_t buffer;
  readRegisters(address, &buffer, 1);
  return buffer; 
}

void SerialBusDevice::writeRegisters(uint8_t startAddress, uint8_t* source, size_t length) {
  auto buffer = new uint8_t[length + 1]();
  buffer[0] = startAddress;
  memcpy(&buffer[1], source, length);
  serialBus.write(deviceAddress, &buffer[0], length + 1);
}

void SerialBusDevice::readRegisters(uint8_t startAddress, uint8_t* destination, size_t length) {
  serialBus.write(deviceAddress, &startAddress, 1, SerialBus::none);
  serialBus.read(deviceAddress, destination, length);
}
