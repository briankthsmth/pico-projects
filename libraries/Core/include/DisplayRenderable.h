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

#include <cstdint>
namespace Core {

///
/// \brief Abstract base class for display render code.
///
class DisplayRenderable {
public:
  ///
  /// \brief Structure for an area on a display to render data into.
  ///
  struct RenderArea {
    /// \brief The area's start column.
    uint8_t startColumn;
    /// \brief The area's end column.
    uint8_t endColumn;
    /// \brief The area's start page.
    uint8_t startPage;
    /// \brief The area's end page.
    uint8_t endPage;

    int getBufferLength();
  };
  
  ///
  /// \brief Properties that define a display.
  ///
  struct Properties {
    /// \brief Display width. 
    /// \description Display width which corresponds to the number of columns.
    uint8_t width;
    /// \brief Display height.
    /// \description Display height which corresponds to the number of segments.
    uint8_t height;
    /// \brief Display maximum pages.
    /// \description Display maximum pages which is usually the height divided by 8. 
    ///   Monochrome display controllers will take a byte of data and map them to 
    ///   8 segments, and a page is these 8 segments across the display columns.
    uint8_t maxPages;
  };

  DisplayRenderable() = default;
  DisplayRenderable(const DisplayRenderable&) = delete;
  ~DisplayRenderable() = default;
  
  ///
  /// \brief Renders the data into a render area.
  /// 
  /// \param data A pointer to the data buffer to render.
  /// \param area The render area on the display.
  ///
  virtual void render(uint8_t* data, RenderArea area) = 0;
  ///
  /// \brief Clears the entire display.
  ///
  virtual void clear() = 0;
  ///
  /// \brief Accessor to the display's properties.
  ///
  virtual Properties getProperties() const = 0;
};

} // namespace Core