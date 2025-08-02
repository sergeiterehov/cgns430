#include "GLog.h"

GLog::GLog(Adafruit_GFX *tft, Print *serial) : tft(tft), serial(serial)
{
  target = tft;
}

size_t GLog::write(uint8_t ch)
{
  if (target == tft)
  {
    if (tft->getCursorY() >= tft->height())
    {
      tft->fillScreen(0);
      tft->setCursor(0, 0);
    }
  }
  return target->write(ch);
}