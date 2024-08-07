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
// Created by Brian Smith 05/03/2024
//

#include "Display.h"
#include "FontManager.h"

#include "hardware/i2c.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
 
 
constexpr uint8_t address = 0x3d;

constexpr uint8_t displayHeight = 48;
constexpr uint8_t displayWidth = 64;

constexpr uint8_t startColumn = 32;
constexpr uint8_t endColumn = startColumn + displayWidth - 1; 
constexpr uint8_t maximumCharacters = displayWidth / 8;
constexpr uint8_t pageHeight = 8; // bits
constexpr uint8_t numberOfPages = displayHeight / pageHeight;
constexpr uint16_t dispalyableBufferLength = numberOfPages * displayWidth;

using namespace LightMeter;

void Display::init() {
  uint8_t commandList[] = {setDisplayOff,
                           setMemoryMode,
                           0x00,
                           setDisplayStartLine | 0x00,
                           setSegmentRemapFlipped,
                           setMuxRatio,
                           displayHeight - 1,
                           setCOMOutputScanRemappedMode,
                           setDisplayOffset,
                           0x00,
                           setCOMPinsConfiguration,
                           0x12,
                           setDisplayClockOscillator,
                           0x80,
                           setPrechargePeriod,
                           0xf1,
                           setVCOMDeselectLevel,
                           0x40,
                           setConstrast,
                           0x8f,
                           setChargePump,
                           0x14,
                           setToNormalDisplay};

  sendCommandList(&commandList[0], sizeof(commandList));
  
  RenderArea clear_area = {0, 127, 0, 7};
  uint8_t clear_buffer[8 * 128];
  std::memset(&clear_buffer[0], 0, sizeof(clear_buffer));
  render(&clear_buffer[0], clear_area);

  sendCommand(setDisplayOn);
}

void Display::render(uint8_t *data, RenderArea area) {
  int length = area.getBufferLength();
  uint8_t commandList[] = {setColumnAddress, area.startColumn, area.endColumn,
                           setPageAddress,   area.startPage,   area.endPage};

  sendCommandList(&commandList[0], sizeof(commandList));
  sendData(data, length);
}

int Display::RenderArea::getBufferLength() {
  int numberOfColumns = static_cast<int>(endColumn - startColumn + 1);
  int numberOfPages = static_cast<int>(endPage - startPage + 1);
  int length = numberOfColumns * numberOfPages;
  return length;
}

void Display::draw(float number, int line) {
  char number_string[maximumCharacters + 1];

  const char *format = number > 1000 ? "%.0f" : "%.2f";
  snprintf(&number_string[0], maximumCharacters + 1, format, number);
  draw(&number_string[0], line);
}

void Display::draw(int number, int line) {
  static const char* format = "%d";

  char number_string[maximumCharacters + 1];
  snprintf(&number_string[0], maximumCharacters + 1, format, number);
  draw(&number_string[0], line);  
}

void Display::draw(const char* string, int line) {
  char full_line_string[maximumCharacters + 1];
  snprintf(&full_line_string[0], maximumCharacters + 1, "%*s",
           maximumCharacters, string);

  FontManager font_manager;
  auto display_line = font_manager.buildLine(full_line_string);
  Display::RenderArea line_area = {startColumn, endColumn,
                                   static_cast<uint8_t>(line * 2),
                                   static_cast<uint8_t>(line * 2 + 1)};
  render(&display_line.image[0][0], line_area);
}

//
// Private Interface
//
void Display::sendCommandList(uint8_t *list, int number) {
  for (int i = 0; i < number; ++i) {
    sendCommand(list[i]);
  }
}

void Display::sendCommand(uint8_t command) {
  uint8_t buffer[] = {0x80, command};
  int count_sent = i2c_write_blocking(i2c_default, address, buffer, 2, false);
}

void Display::sendData(uint8_t *buffer, int length) {
  uint8_t *send_buffer = new uint8_t[length + 1];
  send_buffer[0] = 0x40;
  std::memcpy(&send_buffer[1], buffer, length);
  int count_sent = i2c_write_blocking(i2c_default, address, send_buffer, length + 1, false);
  delete[] send_buffer;
}