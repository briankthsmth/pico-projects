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
// Created by Brian Smith 07/23/2024
//

#include "Af128x64FeatherMonoDisplayDevice.h"

#include <cstdint>

constexpr uint8_t busAddress = 0x3c;

constexpr uint8_t setColumnLowNibbleAddressCommand = 0x00; // Set in lower nibble
constexpr uint8_t setColumnHighNibbleAddressCommand = 0x10; // Set in lower nibble
constexpr uint8_t setMemoryPageAddressingModeCommand = 0x20;
constexpr uint8_t setMemoryVerticalAddressingModeCommand = 0x21;
constexpr uint8_t setConstrastSettingCommand = 0x81; // Needs extra data byte
constexpr uint8_t setSegmentRemapDownCommand = 0xa0;
constexpr uint8_t setSegmentRemapUpCommand = 0xa1;
constexpr uint8_t setToAllNormalCommand = 0xa4;
constexpr uint8_t setToAllOnCommand = 0xa5;
constexpr uint8_t setToNormalDisplayCommand = 0xa6;
constexpr uint8_t setToInverseDisplayCommand = 0xa7;
constexpr uint8_t setMultiplexRationCommand = 0xa8; // Needs extra data byte
constexpr uint8_t setDcToDcSettingModeCommand = 0xad;
constexpr uint8_t displayOffCommand = 0xae;
constexpr uint8_t displayOnCommand = 0xaf;
constexpr uint8_t setPageAddressCommand = 0xb0; // Set in lower nibble
constexpr uint8_t setCommonOutputScanDirectionCommand = 0xc0; // Set in lower nibble
constexpr uint8_t setDisplayOffsetCommand = 0xd3; // Needs extra data byte
constexpr uint8_t setRatioAndFrequencyModeCommand = 0xd5; // Needs extra data byte
constexpr uint8_t setDisChargeAndPreChargePeriodModeCommand = 0xd9; // Needs extra data byte
constexpr uint8_t setVCOMDeselectLevelCommand = 0xdb; // Needs extra data byte
constexpr uint8_t setDisplayStartLineCommand = 0xdc;  // Needs extra data byte
constexpr uint8_t nopCommand = 0xe3;
constexpr uint8_t readModifyWriteStartCommand = 0xe0;
constexpr uint8_t readModifyWriteEndCommand = 0xee;

using namespace Device;
using namespace Core;

Af128x64FeatherMonoDisplayDevice::Af128x64FeatherMonoDisplayDevice(
    Core::SerialBus &bus)
    : SerialBusDevice(bus, busAddress) {}

void Af128x64FeatherMonoDisplayDevice::init() {
  uint8_t commandList[] = {displayOffCommand,
                           setColumnLowNibbleAddressCommand,
                           setColumnHighNibbleAddressCommand,
                           setPageAddressCommand,
                           setDisplayStartLineCommand,
                           0x00,
                           setConstrastSettingCommand,
                           0x6e,
                           setMemoryPageAddressingModeCommand,
                           setSegmentRemapDownCommand,
                           setCommonOutputScanDirectionCommand,
                           setToAllNormalCommand,
                           setToNormalDisplayCommand,
                           setMultiplexRationCommand,
                           0x3f,
                           setDisplayOffsetCommand,
                           0x60,
                           setRatioAndFrequencyModeCommand,
                           0x41,
                           setDisChargeAndPreChargePeriodModeCommand,
                           0x22,
                           setVCOMDeselectLevelCommand,
                           0x35,
                           setDcToDcSettingModeCommand,
                           0x80, // external Vpp used
                           displayOnCommand};

  sendCommandList(&commandList[0], sizeof(commandList));
}

void Af128x64FeatherMonoDisplayDevice::render(
    uint8_t *data, DisplayRenderable::RenderArea area) {}

void Af128x64FeatherMonoDisplayDevice::sendCommandList(uint8_t *commands,
                                                       int length) 
{
  int writeLength = 2 * length;
  uint8_t *buffer = new uint8_t[writeLength];
  for (int commandIndex = 0; commandIndex < length; ++commandIndex) {
    int controlIndex = 2 * commandIndex;
    int dataIndex = controlIndex + 1;
    buffer[controlIndex] = commandIndex < (length - 1) ? 0x80 : 0x00;
    buffer[dataIndex] = commands[commandIndex];
  }
  serialBus.write(deviceAddress, &buffer[0], writeLength);
  delete[] buffer;
}

void Af128x64FeatherMonoDisplayDevice::writeDisplayData(uint8_t startPage,
                                                        uint8_t startAddress,
                                                        uint8_t *source,
                                                        int length) 
{
  uint8_t lowNibbleAddressCommand = setColumnLowNibbleAddressCommand & (0x0f & startAddress);
  uint8_t highNibbleAddressCommand = setColumnHighNibbleAddressCommand & (0x07 & (startAddress >> 4));
  uint8_t pageCommand = setPageAddressCommand & (0x0f & startPage);
  uint8_t addressCommandList[] = {
    lowNibbleAddressCommand,
    highNibbleAddressCommand,
    pageCommand
  };
  sendCommandList(addressCommandList, sizeof(addressCommandList));
}