// Copyright 2019 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Arduino sample project to use jvsio library.
// See also https://github.com/toyoshim/iona, and
// https://github.com/toyoshim/iona-346 to have this library
// as a git submodule.

#include "JVSIO.h"
#include "NanoClient.h"
// JVS pins for Arduino Nano/Uno
//  D0  - JVS Data+  => USB Type B Pin 3 (D+ Pin in USB proper use)
//  D2  - JVS Data-  => USB Type B Pin 2 (D- Pin in USB proper use)
//  D3  - JVS Sense  => USB Type B Pin 1 (5V Pin in USB proper use)
//  D13 - LED
NanoDataClient data;
NanoSenseClient sense;
NanoLedClient led;
JVSIO io(&data, &sense, &led);

static const char id[] = "SEGA ENTERPRISES,LTD.compat;Sample for Arduino NANO/UNO";

void setup() {
  Serial.begin(115200);
  Serial.println(id);
  io.begin();
}

void loop() {
  uint8_t len;
  uint8_t* data = io.getNextCommand(&len);
  if (!data)
    return;

  switch (*data) {
   case JVSIO::kCmdIoId:
    io.pushReport(JVSIO::kReportOk);
    for (size_t i = 0; id[i]; ++i)
      io.pushReport(id[i]);
    io.pushReport(0);
    break;
   case JVSIO::kCmdFunctionCheck:
    io.pushReport(JVSIO::kReportOk);

    io.pushReport(0x01);  // sw
    io.pushReport(0x02);  // players
    io.pushReport(0x0C);  // buttons
    io.pushReport(0x00);

    io.pushReport(0x03);  // analog inputs
    io.pushReport(0x08);  // channels
    io.pushReport(0x00);  // bits
    io.pushReport(0x00);

    io.pushReport(0x12);  // general purpose driver
    io.pushReport(0x08);  // slots
    io.pushReport(0x00);
    io.pushReport(0x00);

    io.pushReport(0x02);  // coin
    io.pushReport(0x02);  // slots
    io.pushReport(0x00);
    io.pushReport(0x00);

    io.pushReport(0x00);
    break;
   case JVSIO::kCmdSwInput:
    io.pushReport(JVSIO::kReportOk);
    io.pushReport(0x00);  // TEST, TILT1-3, and undefined x4.
    for (size_t player = 0; player < data[1]; ++player) {
      for (size_t line = 1; line <= data[2]; ++line)
        io.pushReport(0x00);
    }
    break;
   case JVSIO::kCmdCoinInput:
    io.pushReport(JVSIO::kReportOk);
    for (size_t slot = 0; slot < data[1]; ++slot) {
      io.pushReport(0x00);
      io.pushReport(0x00);
    }
    break;
   case JVSIO::kCmdAnalogInput:
    io.pushReport(JVSIO::kReportOk);
    for (size_t channel = 0; channel < data[1]; ++channel) {
      io.pushReport(0x80);
      io.pushReport(0x00);
    }
    break;
   case JVSIO::kCmdCoinSub:
    io.pushReport(JVSIO::kReportOk);
    break;
   case JVSIO::kCmdDriverOutput:
    io.pushReport(JVSIO::kReportOk);
    break;
  }
}
