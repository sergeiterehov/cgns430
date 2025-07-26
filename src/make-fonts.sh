#!/bin/sh
lv_font_conv --bpp 1 --format lvgl --lv-include "lvgl.h" --no-compress --no-prefilter  --font Martian_Mono/static/MartianMono-Regular.ttf --range 0x20-0x7F --size 10 -o font_10.c
lv_font_conv --bpp 1 --format lvgl --lv-include "lvgl.h" --no-compress --no-prefilter  --font Martian_Mono/static/MartianMono-SemiBold.ttf --range 0x20-0x7F --size 20 -o font_20.c
