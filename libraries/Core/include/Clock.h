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
// Created by Brian Smith 07/18/2024
//

#pragma once

#include <cstdint>
#include <ctime>
#include <compare>

namespace Core {

  struct Time final {
    /// \brief Seconds for the time in the range of 0...59.
    uint8_t seconds;
    /// \brief Minutes for the time in the range of 0...59.
    uint8_t minutes;
    /// \brief The hour in a day for the time in the range 0...23. 
    uint8_t hour;
    
    uint32_t inSeconds() const {
      return ((static_cast<uint32_t>(hour) * 60.0) + static_cast<uint32_t>(minutes)) * 60.0 
        + static_cast<uint32_t>(seconds);
    }
    
    friend auto operator<=>(const Time& time, const Time& other) {
      return time.inSeconds() <=> other.inSeconds();
    }
  }; // struct Time
  
  struct Date final {
    uint8_t dayOfWeek;
    uint8_t dayOfMonth;
    uint8_t month;
    uint8_t year;
  }; // struct Date
  
  struct ClockDatum final {
    Time time;
    Date date;
    
    char* toString() {
      std::tm tm{};
      tm.tm_sec = time.seconds;
      tm.tm_min = time.minutes;
      tm.tm_hour = time.hour;
      tm.tm_wday = date.dayOfWeek - 1;
      tm.tm_mday = date.dayOfMonth;
      tm.tm_mon = date.month - 1;
      tm.tm_year = date.year + 100; // From 1900.
      return std::asctime(&tm);
    }
  }; // struct ClockDatum

} // namespace Core