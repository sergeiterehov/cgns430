#include "global.h"

SPIClass hspi = SPIClass(HSPI);
Adafruit_ILI9341 tft = Adafruit_ILI9341(&hspi, TFT_DC, TFT_CS, TFT_RST);

GLog glog(&tft, &Serial);

DrefProvider drefProvider;
Navigraph navigraph(&glog);
