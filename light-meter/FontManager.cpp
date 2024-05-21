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
// Created by Brian Smith 05/17/2024
//

#include "FontManager.h"
#include "Glyph.h"
#include "font_13x8.h"
#include <cstdint>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace LightMeter;

Glyph FontManager::getGlyph(uint8_t character) {
  uint8_t* raw_glyph = &font[character - ' '][0];
  Glyph glyph;

  for (int row = 0; row < 2; ++row) {
    uint8_t mask = 0x80;
    uint8_t bit = 7;
    for(int column = 0; column < 8; ++column) { 
      uint8_t datum = 0x00;
      int shift = bit - 7;
      for(int i = (row == 0 ? 8 : 0); i < (row == 0 ? 13 : 8); ++i) {
        if (shift >= 0) {
          datum |= (raw_glyph[i] & mask) >> shift;
        } else {
          datum |= (raw_glyph[i] & mask) << abs(shift);
        }
        shift += 1;
      }
      glyph.image[row][column] = datum;
      mask = mask >> 1;
      --bit;
    }
  }
  return glyph;
}

DisplayLine FontManager::buildLine(const char* string) {
  DisplayLine line;
  
  for (int i = 0; i < 8 && string[i] != 0; i++) {
    auto glyph = getGlyph(string[i]);
    memcpy(&line.image[0][i * 8], &glyph.image[0][0], 8);
    memcpy(&line.image[1][i * 8], &glyph.image[1][0], 8);
  }
  return line;
}