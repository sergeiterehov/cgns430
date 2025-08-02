#pragma once

#include <cmath>
#include <utility>

// Константы
extern const double EARTH_RADIUS_M;
extern const double NM_TO_METERS;

// Объявления функций
double deg2rad(double deg);
double rad2deg(double rad);
double mercY(double lat_rad);
double mercYInv(double y);
std::pair<int, int> projectToScreen(
    double lon, double lat,
    double clonx, double claty,
    double radius_nm,
    int width = 250, int height = 250);
