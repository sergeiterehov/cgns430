// projection.cpp
#include "projection.h"
#include <algorithm>

// Определения констант
const double PI = M_PI;
const double EARTH_RADIUS_M = 6371000.0;
const double NM_TO_METERS = 1852.0;

// Реализации функций
double deg2rad(double deg)
{
  return deg * PI / 180.0;
}

double rad2deg(double rad)
{
  return rad * 180.0 / PI;
}

double mercY(double lat_rad)
{
  return log(tan(PI / 4.0 + lat_rad / 2.0));
}

double mercYInv(double y)
{
  return 2.0 * atan(exp(y)) - PI / 2.0;
}

std::pair<int, int> projectToScreen(
    double lon, double lat,
    double clonx, double claty,
    double radius_nm,
    int width, int height)
{
  double clat_rad = deg2rad(claty);
  double clon_rad = deg2rad(clonx);

  double radius_m = radius_nm * NM_TO_METERS;
  double meters_per_deg_lat = 111319.9;
  double meters_per_deg_lon = meters_per_deg_lat * cos(clat_rad);

  double delta_lon_deg = radius_m / meters_per_deg_lon;
  double delta_lat_deg = radius_m / meters_per_deg_lat;

  double lon_min = clonx - delta_lon_deg;
  double lon_max = clonx + delta_lon_deg;
  double lat_min = claty - delta_lat_deg;
  double lat_max = claty + delta_lat_deg;

  double lon_rad = deg2rad(lon);
  double lat_rad = deg2rad(lat);

  double merc_lat = mercY(lat_rad);
  double merc_lat_min = mercY(deg2rad(lat_min));
  double merc_lat_max = mercY(deg2rad(lat_max));

  double x = (lon_rad - deg2rad(lon_min)) / (deg2rad(lon_max) - deg2rad(lon_min)) * width;
  double y = (merc_lat_max - merc_lat) / (merc_lat_max - merc_lat_min) * height;

  // x = std::max(0.0, std::min(static_cast<double>(width - 1), x));
  // y = std::max(0.0, std::min(static_cast<double>(height - 1), y));

  return {static_cast<int>(x), static_cast<int>(y)};
}