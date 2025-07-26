#pragma once

#include "Print.h"

#include "Adafruit_GFX.h"

class GLog : public Print
{
  Print *target;

  Adafruit_GFX *tft;
  Print *serial;

public:
  GLog(Adafruit_GFX *tft, Print *serial);

  virtual size_t write(uint8_t);

  void useSerial()
  {
    target = serial;
  }

  void useTFT()
  {
    target = tft;
  }
};
