#include <map>

#include "Arduino.h"
#include <lvgl.h>

#include "projection.h"
#include "colors.h"
#include "global.h"

#include "imgs/plane.c"

class NavMapPage
{
private:
  lv_obj_t *container;
  lv_obj_t *obj_points;
  lv_obj_t *img_plane;
  lv_obj_t *label_range;

  std::map<NearestPoint *, lv_obj_t *> map_point_to_obj;

  float heading = 0.0;

  float c_lonx = 37.90;
  float c_laty = 55.40;
  float range = 1.0;

  // Размеры контейнера
  float scale = 250;

  std::pair<int, int> project(float lonx, float laty)
  {
    return projectToScreen(lonx, laty, c_lonx, c_laty, range / 2.0);
  }

public:
  NavMapPage(lv_obj_t *parent)
  {
    container = lv_obj_create(parent);
    {
      auto lv = container;

      lv_obj_remove_style_all(lv);
      lv_obj_set_style_bg_color(lv, color_black, 0);
      lv_obj_set_style_bg_opa(lv, LV_OPA_COVER, 0);
      lv_obj_set_size(lv, lv_pct(100), lv_pct(100));
    }

    obj_points = lv_obj_create(parent);
    {
      auto lv = obj_points;

      lv_obj_remove_style_all(lv);
      lv_obj_set_size(lv, scale, scale);
    }

    label_range = lv_label_create(container);
    {
      auto lv = label_range;
      lv_label_set_text(lv, "_nm");
      lv_obj_set_style_text_font(lv, &font_10, 0);
      lv_obj_set_style_text_color(lv, color_white, 0);
      lv_obj_align(lv, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    }

    {
      // lv_obj_t *line = lv_line_create(container);
      // static lv_point_precise_t route_points[] = {
      //     {100, 100},
      //     {150, 150},
      //     {200, 100},
      //     {250, 150}};
      // lv_line_set_points(line, route_points, 4);
      // lv_obj_set_style_line_width(line, 2, 0);
      // lv_obj_set_style_line_color(line, lv_color_hex(0x00FF00), 0);
      // lv_obj_align(line, LV_ALIGN_CENTER, 0, 0);
    }

    // Используйте его в объекте изображения
    img_plane = lv_image_create(container);
    {
      auto lv = img_plane;
      lv_image_set_src(lv, &plane);
      lv_obj_set_size(lv, 17, 17);
      lv_obj_set_style_image_recolor(lv, color_white, 0);
      lv_obj_set_style_image_recolor_opa(lv, LV_OPA_COVER, 0);
      lv_image_set_pivot(lv, 8, 8);
      lv_obj_align(lv, LV_ALIGN_CENTER, 0, 0);
    }

    {
      lv_anim_t a;
      lv_anim_init(&a);
      lv_anim_set_var(&a, img_plane);
      lv_anim_set_exec_cb(&a, [](void *img, int32_t v)
                          { lv_image_set_rotation((lv_obj_t *)img, v); });
      lv_anim_set_values(&a, 0, 3600);
      lv_anim_set_duration(&a, 5000);
      lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
      lv_anim_start(&a);
    }

    set_range(10.0);
    // set_heading(45.0);

    // lv_refr_now(NULL);

    navigraph.lookup(c_lonx, c_laty, range / 2.0);

    update_points();
  }

  ~NavMapPage()
  {
    lv_obj_del(container);
  }

  void set_center(float lonx, float laty)
  {
    c_lonx = lonx;
    c_laty = laty;

    project_points();
  }

  void set_range(float new_range)
  {
    range = new_range;

    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.1fnm", range);
    lv_label_set_text(label_range, buffer);
  }

  void set_heading(float new_heading)
  {
    heading = new_heading;

    lv_image_set_rotation(img_plane, heading * 10.0);
    lv_obj_move_foreground(img_plane);
  }

  void update_points()
  {
    for (auto &pair : map_point_to_obj)
    {
      lv_obj_del(pair.second);
    }

    map_point_to_obj.clear();

    for (auto &p : navigraph.nearest)
    {
      lv_obj_t *label_point = lv_label_create(obj_points);
      {
        auto lv = label_point;
        lv_label_set_text(lv, p.ident.c_str());
        lv_obj_set_style_text_font(lv, &font_10, 0);
        lv_obj_align(lv, LV_ALIGN_TOP_LEFT, 0, 0);

        map_point_to_obj[&p] = lv;

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

    project_points();
  }

  void project_points()
  {
    for (auto &p : navigraph.nearest)
    {
      auto [x, y] = project(p.lonx, p.laty);

      auto it_label_point = map_point_to_obj.find(&p);

      if (it_label_point == map_point_to_obj.end())
        continue;

      {
        auto lv = it_label_point->second;

        lv_obj_set_pos(lv, x, y);
      }
    }
  }
};
