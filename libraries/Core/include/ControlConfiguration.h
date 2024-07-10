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

#include <cstdint>

namespace Core {

struct ControlConfiguration final {
  struct Time final {
    ///
    /// The hour in a day for the time in the range 0...23. 
    ///
    uint8_t hour;
    ///
    /// Minutes for the time in the range of 0...59.
    ///
    uint8_t minutes;
    ///
    /// Seconds for the time in the range of 0...59.
    ///
    uint8_t seconds;
  }; // struct Time
  
  /// 
  /// The start time in a day to switch a device "on".
  ///
  Time startTime = {0,0,0};
  ///
  /// The end time in a day to switch a device "off".
  ///
  Time endTime = {0, 0, 0};
  ///
  /// A power level for the "on" state for a device in the range of 0.0...1.0. This can
  /// be ignored by a device that only toggles between on and off.
  ///
  float powerLevel = 1.0;
}; // struct ControlConfiguration

}; // namespace Core