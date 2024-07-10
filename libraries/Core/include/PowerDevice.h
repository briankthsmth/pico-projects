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
// Created by Brian Smith 07/09/2024
//

#pragma once

namespace Core {

///
/// \brief Abstract base class for a power device.
/// \description Defines an interface for devices whose power can be controlled.
///   This class can not be created, and creation is intended by subclasses only.
///
class PowerDevice {
public:
  ///
  /// \brief State enumeration for power.
  /// \description Enumeration for the power state as on or off.
  ///
  enum State { on, off };

  PowerDevice() = default;
  PowerDevice(const PowerDevice&) = delete;
  ~PowerDevice() = default;

  virtual void setState(State) = 0;
  
  void setPowerLevel(float level) { powerLevel = level; }
  float getPowerLevel() const { return powerLevel; }
  
  ///
  /// \description Accessor to the status of the device.
  /// \return The state of the device.
  ///
  State getStatus() { return status; }

protected:
  State status = off;
  float powerLevel = 1.0;
}; // class PowerDevice

}; // namespace Core