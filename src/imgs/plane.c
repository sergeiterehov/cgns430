#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_PLANE
#define LV_ATTRIBUTE_IMG_PLANE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_PLANE uint8_t plane_map[] = {
  0x00, 0x80, 0x00, 
  0x01, 0xc0, 0x00, 
  0x01, 0xc0, 0x00, 
  0xff, 0xff, 0x80, 
  0xff, 0xff, 0x80, 
  0xff, 0xff, 0x80, 
  0x01, 0xc0, 0x00, 
  0x01, 0xc0, 0x00, 
  0x01, 0xc0, 0x00, 
  0x01, 0xc0, 0x00, 
  0x01, 0xc0, 0x00, 
  0x00, 0x80, 0x00, 
  0x00, 0x80, 0x00, 
  0x00, 0x80, 0x00, 
  0x07, 0xf0, 0x00, 
  0x07, 0xf0, 0x00, 
  0x00, 0x80, 0x00, 
};

const lv_img_dsc_t plane = {
  .header = {
    .cf = LV_COLOR_FORMAT_A1,
    .w = 17,
    .h = 17,
  },
  .data_size = sizeof(plane_map),
  .data = plane_map,
};
