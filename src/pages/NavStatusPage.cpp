#include "Arduino.h"
#include <lvgl.h>

#include "colors.h"
#include "global.h"

#include "imgs/plane.c"

class NavStatusPage
{
private:
  lv_obj_t *container;
  lv_obj_t *img_plane;

public:
  NavStatusPage(lv_obj_t *parent)
  {
    container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_style_bg_color(container, color_black, 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, 0);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));

    float clonx = 37.90;
    float claty = 55.40;
    float radius = 2.0;

    // Размеры контейнера
    float scale = 250;
    float range = 2.0 * radius / 60.0;

    navigraph.lookup(clonx, claty, radius);

    for (auto &p : navigraph.nearest)
    {
      // Преобразуем географические координаты в пиксельные
      float x = scale / 2.0 + (p.lonx - clonx) / range * scale;
      float y = scale / 2.0 + (claty - p.laty) / range * scale;

      lv_obj_t *label_wp = lv_label_create(container);
      {
        auto lv = label_wp;
        lv_label_set_text(lv, p.ident.c_str());
        lv_obj_set_style_text_font(lv, &font_10, 0);
        lv_obj_align(lv, LV_ALIGN_TOP_LEFT, x, y);

        if (p.type == NavWayPoint)
        {
          lv_obj_set_style_text_color(lv, color_magenta, 0);
        }
        else if (p.type == NavAirport)
        {
          lv_obj_set_style_text_color(lv, color_white, 0);
        }
        else if (p.type == NavVOR)
        {
          lv_obj_set_style_text_color(lv, color_blue, 0);
        }
        else
        {
          lv_obj_set_style_text_color(lv, color_white, 0);
        }
      }
    }

    lv_obj_t *line = lv_line_create(container);
    static lv_point_precise_t route_points[] = {
        {100, 100},
        {150, 150},
        {200, 100},
        {250, 150}};

    lv_line_set_points(line, route_points, 4);
    lv_obj_set_style_line_width(line, 2, 0);
    lv_obj_set_style_line_color(line, lv_color_hex(0x00FF00), 0);
    lv_obj_align(line, LV_ALIGN_CENTER, 0, 0);

    LV_IMG_DECLARE(plane);

    // Используйте его в объекте изображения
    img_plane = lv_img_create(container);
    {
      auto lv = img_plane;
      lv_img_set_src(lv, &plane);
      lv_obj_set_style_image_recolor(lv, color_white, 0);
      lv_obj_set_style_image_recolor_opa(lv, LV_OPA_COVER, 0);
      lv_image_set_pivot(lv, 8, 8);
      lv_obj_align(lv, LV_ALIGN_CENTER, 0, 0);

      lv_anim_t a;
      lv_anim_init(&a);
      lv_anim_set_var(&a, lv);
      lv_anim_set_exec_cb(&a, [](void *img, int32_t v)
                          { lv_image_set_rotation((lv_obj_t *)img, v); });
      lv_anim_set_values(&a, 0, 3600);
      lv_anim_set_duration(&a, 5000);
      lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
      lv_anim_start(&a);
    }
  }

  ~NavStatusPage()
  {
    lv_obj_del(container);
  }
};
