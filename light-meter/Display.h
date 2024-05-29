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
// Display driver for a SSD1306 with a 64x48 OLED monochrome display.
//
// Created by Brian Smith 05/03/2024
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>

// commands
constexpr uint8_t setMemoryMode                 = 0x20;
constexpr uint8_t setColumnAddress              = 0x21;
constexpr uint8_t setPageAddress                = 0x22;
constexpr uint8_t setHorizontalScroll           = 0x26;
constexpr uint8_t deactivateScroll              = 0x2e;
constexpr uint8_t activateScroll                = 0x2f;
constexpr uint8_t setDisplayStartLine           = 0x40;
constexpr uint8_t setConstrast                  = 0x81;
constexpr uint8_t setChargePump                 = 0x8d;
constexpr uint8_t setSegmentRemapDefault        = 0xa0;
constexpr uint8_t setSegmentRemapFlipped        = 0xa1;
constexpr uint8_t setEntireDisplayResume        = 0xa4; // Sets every pixel to resume following RAM
constexpr uint8_t setEntireDisplayOn            = 0xa5; // Sets every pixel on regardless of RAM
constexpr uint8_t setToNormalDisplay            = 0xa6;
constexpr uint8_t setToInverseDisplay           = 0xa7;
constexpr uint8_t setMuxRatio                   = 0xa8;
constexpr uint8_t setDisplayOff                 = 0xae;
constexpr uint8_t setDisplayOn                  = 0xaf;
constexpr uint8_t setCOMOutputScanNormalMode    = 0xc0;
constexpr uint8_t setCOMOutputScanRemappedMode  = 0xc8;
constexpr uint8_t setDisplayOffset              = 0xd3;
constexpr uint8_t setDisplayClockOscillator     = 0xd5;
constexpr uint8_t setPrechargePeriod            = 0xd9;
constexpr uint8_t setCOMPinsConfiguration       = 0xda;
constexpr uint8_t setVCOMDeselectLevel          = 0xdb;

namespace LightMeter {

class Display {
public:
  struct RenderArea {
    uint8_t startColumn;
    uint8_t endColumn;
    uint8_t startPage;
    uint8_t endPage;

    int getBufferLength();
  };

  Display() = default;
  ~Display() = default;

  void init();

  void render(uint8_t *, RenderArea);

  /*! Draw a floating point number at a given line number.
   */
  void draw(float, int);

private:
  static void sendCommandList(uint8_t *, int);
  static void sendCommand(uint8_t);
  static void sendData(uint8_t *, int);
};

}; // namespace LightMeter

#endif // DISPLAY_H