#include "Arduino.h"
#include <lvgl.h>

#include "colors.h"
#include "global.h"
#include "pages/NavStatusPage.cpp"

class Screen
{
private:
  lv_obj_t *parent;
  lv_obj_t *label_com_frq;
  lv_obj_t *label_com_stb_frq;
  lv_obj_t *box_page;

  NavStatusPage *page;

  void _handle_com_frq(float raw)
  {
    char buffer[10];
    sprintf(buffer, "%07.3f", raw / 1000);

    lv_label_set_text(label_com_frq, buffer);
  }

  void _handle_com_stb_frq(float raw)
  {
    char buffer[10];
    sprintf(buffer, "%07.3f", raw / 1000);

    lv_label_set_text(label_com_stb_frq, buffer);
  }

public:
  Screen(lv_obj_t *_parent)
  {
    parent = _parent;

    // Черная нижняя панель
    lv_obj_t *bottom_bar = lv_obj_create(parent);
    lv_obj_remove_style_all(bottom_bar);
    lv_obj_set_size(bottom_bar, 320, 24);
    lv_obj_align(bottom_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_border_color(bottom_bar, color_cyan, 0);
    lv_obj_set_style_border_width(bottom_bar, 1, 0);
    lv_obj_set_style_border_side(bottom_bar, LV_BORDER_SIDE_TOP, 0);

    lv_obj_t *status_box = lv_obj_create(bottom_bar);
    lv_obj_remove_style_all(status_box);
    lv_obj_set_size(status_box, 180, 24);
    lv_obj_align(status_box, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(status_box, color_black, 0);
    lv_obj_set_style_bg_opa(status_box, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(status_box, color_cyan, 0);
    lv_obj_set_style_border_width(status_box, 1, 0);
    lv_obj_set_style_border_side(status_box, LV_BORDER_SIDE_RIGHT, 0);

    lv_obj_t *label_msg = lv_label_create(status_box);
    {
      auto lv = label_msg;
      lv_label_set_text(lv, "MSG");
      lv_obj_set_style_text_color(lv, color_black, 0);
      lv_obj_set_style_bg_color(lv, color_yellow, 0);
      lv_obj_set_style_bg_opa(lv, LV_OPA_COVER, 0);
      lv_obj_set_style_text_font(lv, &font_20, 0);
      lv_obj_align(lv, LV_ALIGN_TOP_RIGHT, -5, 0);
    }

    // Голубая рамка (имитация drawRect)
    lv_obj_t *box_com = lv_obj_create(parent);
    lv_obj_remove_style_all(box_com);
    lv_obj_set_size(box_com, 100, 72);
    lv_obj_set_style_border_color(box_com, color_cyan, 0);
    lv_obj_set_style_border_width(box_com, 1, 0);
    lv_obj_set_style_bg_opa(box_com, LV_OPA_TRANSP, 0); // прозрачный фон
    lv_obj_align(box_com, LV_ALIGN_TOP_LEFT, 0, 0);

    // Метка "COM"
    lv_obj_t *label_com = lv_label_create(box_com);
    lv_label_set_text(label_com, "COM");
    lv_obj_set_style_text_color(label_com, color_cyan, 0);
    lv_obj_set_style_text_font(label_com, &font_10, 0);
    lv_obj_align(label_com, LV_ALIGN_TOP_LEFT, 4, 4);

    // Com active
    label_com_frq = lv_label_create(box_com);
    lv_label_set_text(label_com_frq, "000.000");
    lv_obj_set_style_text_color(label_com_frq, color_white, 0);
    lv_obj_set_style_text_font(label_com_frq, &font_20, 0);
    lv_obj_align(label_com_frq, LV_ALIGN_TOP_LEFT, 4, 16);

    // Com standby
    label_com_stb_frq = lv_label_create(box_com);
    lv_label_set_text(label_com_stb_frq, "000.000");
    lv_obj_set_style_text_color(label_com_stb_frq, color_cyan, 0);
    lv_obj_set_style_bg_color(label_com_stb_frq, color_cyan, 0);
    lv_obj_set_style_text_font(label_com_stb_frq, &font_20, 0);
    lv_obj_align(label_com_stb_frq, LV_ALIGN_TOP_LEFT, 4, 40);

    // Page
    box_page = lv_obj_create(parent);
    {
      auto lv = box_page;
      lv_obj_remove_style_all(lv);
      lv_obj_set_size(lv, 220, 216);
      lv_obj_align(lv, LV_ALIGN_TOP_LEFT, 100, 0);
    }

    page = new NavStatusPage(box_page);

    drefProvider.addListener(
        "sim/cockpit2/radios/actuators/com2_frequency_hz_833",
        [this](float v)
        { this->_handle_com_frq(v); });
    drefProvider.addListener(
        "sim/cockpit2/radios/actuators/com2_standby_frequency_hz_833",
        [this](float v)
        { this->_handle_com_stb_frq(v); });
    // sim/cockpit2/radios/actuators/nav2_frequency_hz
    // sim/cockpit2/radios/actuators/nav2_standby_frequency_hz
  }

  void change_active(bool is_com)
  {
    if (is_com)
    {
      lv_obj_set_style_text_color(label_com_stb_frq, color_blue, 0);
      lv_obj_set_style_bg_opa(label_com_stb_frq, LV_OPA_COVER, 0);
    }
    else
    {
      lv_obj_set_style_text_color(label_com_stb_frq, color_cyan, 0);
      lv_obj_set_style_bg_opa(label_com_stb_frq, LV_OPA_TRANSP, 0);
    }
  }
};
