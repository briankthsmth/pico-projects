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

#include <cstddef>
#include <cstdint>

namespace Core {

class SerialBus;

///
/// \brief Abstract base class for devices that use a I2C bus.
///
class SerialBusDevice {
public:
  SerialBusDevice(SerialBus &bus, uint8_t address) : serialBus(bus), deviceAddress(address) {}
  virtual ~SerialBusDevice() = 0;

  void writeRegister(uint8_t address, uint8_t data);
  uint8_t readRegister(uint8_t address);
  void writeRegisters(uint8_t startAddress, uint8_t* source, size_t length);
  void readRegisters(uint8_t startAddress, uint8_t* destination, size_t length);

protected:
  /// \brief Property for sub-classes to access the I2C bus.
  SerialBus& serialBus;
  /// \brief Bus address for a physical device.
  uint8_t deviceAddress;
}; // class SerialBusDevice

}; // namespace Core