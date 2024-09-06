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

#include "Clock.h"

namespace Core {

///
/// \brief Abstract base class for date/time devices.
/// \Description Defines an interface for devices that provide date and time
///   information.
///
class RealTimeClockDevice {
public:
  
  RealTimeClockDevice() = default;
  RealTimeClockDevice(const RealTimeClockDevice&) = delete;
  ~RealTimeClockDevice() = default;
  
  ///
  /// \brief Abstract method to read the current time.
  ///
  /// \return A Time structure with the current time.
  ///
  virtual Time readTime() = 0;
  ///
  /// \brief Abstract method to read the current date.
  ///
  /// \return A Date structure with the current date.
  ///
  virtual Date readDate() = 0;
  ///
  /// \brief Abstract method to read the current date and time.
  ///
  /// \return A Date structure with the current date and time.
  ///
  virtual ClockDatum read() = 0;
  
  ///
  /// \brief Abstract method to write to a device a datum of clock time.
  /// 
  /// \param A ClockDatum with the date and time to write.
  ///
  virtual void write(ClockDatum clockDatum) = 0;
}; // class RealTimeClockDevice

}; // namespace Core