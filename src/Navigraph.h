#pragma once

#include <vector>
#include "Print.h"

#include "Arduino.h"
#include <sqlite3.h>

enum NavPointType
{
  NavWayPoint,
  NavAirport,
  NavVOR,
};

struct NearestPoint
{
  NavPointType type;
  std::string ident;
  float lonx;
  float laty;
};

class Navigraph
{
private:
  char sql[512];
  sqlite3 *db;

  Print *log;

  void exec_nearest(NavPointType type);

public:
  Navigraph(Print *log);

  std::vector<NearestPoint> nearest;
  bool begin(const char *filename);

  void lookup(float lonx, float laty, float r);
};
