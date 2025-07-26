#include <rom/crc.h>

#include <WiFi.h>
#include <AsyncUDP.h>
#include "SD.h"
#include "FS.h"
#include "SPI.h"

#include "lvgl.h"

#include "Adafruit_GFX.h"
#include <sqlite3.h>

#include "global.h"
#include "Screen.cpp"

#include "secrets.h"
#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif
#ifndef WIFI_PASS
#define WIFI_PASS ""
#endif

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint16_t draw_buf[DRAW_BUF_SIZE / sizeof(uint16_t)];

lv_display_t *display;

void flush_display_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map)
{
  tft.drawRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
  lv_display_flush_ready(display);
}

Screen *screen;

#ifdef GNS_ONLINE
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;
const int localUdpPort = 30000;
#endif

void setup()
{
  Serial.begin(115200);
  Serial.println("CGNS430");

  pinMode(TFT_BACKLIGHT, OUTPUT);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(0);
  digitalWrite(TFT_BACKLIGHT, HIGH);

#ifdef GNS_BOOT_LOGO
  tft.setCursor((tft.width() - 18 * 7) / 2, 100);
  tft.setTextSize(3);
  tft.println("CGNS430");
  tft.setTextSize(1);
  tft.setCursor((tft.width() - 6 * 17) / 2, 130);
  tft.println("Terekhov Research");
  delay(2000);
#endif

  tft.fillScreen(0);
  tft.setCursor(0, 0);
  tft.setTextSize(1);

  glog.println("CGNS430 v0.1!");

#ifdef GNS_ONLINE
  glog.print("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  glog.println("OK");

  IPAddress addr = WiFi.localIP();
  glog.printf("IP: %d.%d.%d.%d\n", addr[0], addr[1], addr[2], addr[3]);

  glog.printf("Listening port %i...", localUdpPort);
  if (drefProvider.begin(localUdpPort) == 0)
  {
    glog.println("ERROR");
    return;
  }
  glog.println("OK");

  glog.print("Waiting DREFs...");
  while (drefProvider.counter == 0)
    delay(500);
  glog.println("OK");
#else
  glog.println("Offline mode");
#endif

  glog.print("SD init...");
  SD.end();
  delay(100);
  if (!SD.begin(SD_CS, SPI, 80000000))
  {
    glog.println("ERROR");
    return;
  }
  glog.println("OK");

  glog.printf("Card size: %llu MB\n", SD.cardSize() / (1024 * 1024));

  glog.print("sqlite init...");
  if (sqlite3_initialize() != SQLITE_OK)
  {
    glog.println("ERROR");
    return;
  }
  glog.println("OK");

  glog.print("Opening AIRAC...");
  {
    if (!navigraph.begin("/sd/CGNS430/little_navmap_navigraph.sqlite"))
    {
      glog.println("ERROR");
      return;
    }
  }
  glog.println("OK");

  glog.println("Starting graphics...");

  lv_init();

  display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);

  lv_display_set_buffers(display, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(display, flush_display_cb);
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);

  lv_obj_t *lv_screen = lv_screen_active();
  lv_obj_set_style_bg_color(lv_screen, color_blue, 0);

  screen = new Screen(lv_screen);

  glog.useSerial();
}

unsigned long frame_millis = 0;
unsigned long last_frame_millis = 0;

void loop(void)
{
  frame_millis = millis();

  drefProvider.loop();

  lv_tick_inc(frame_millis - last_frame_millis);
  last_frame_millis = frame_millis;
  lv_task_handler();
}
