#pragma once

// #define GNS_BOOT_LOGO
// #define GNS_ONLINE

#define SD_CS 5

#define TFT_DC 2  // Data/command pin
#define TFT_CS 15 // Chip-select pin
#define TFT_RST 0 // Reset pin
#define TFT_BACKLIGHT 27

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

#include "Adafruit_ILI9341.h"

#include "DrefProvider.h"
#include "Navigraph.h"
#include "GLog.h"

extern DrefProvider drefProvider;
extern Navigraph navigraph;
extern Adafruit_ILI9341 tft;
extern GLog glog;