#include "Navigraph.h"

Navigraph::Navigraph(Print *log) : log(log) {}

bool Navigraph::begin(const char *filename)
{
  int err = sqlite3_open(filename, &db);
  if (err)
  {
    log->printf("sql error: %s\n", sqlite3_errmsg(db));
    return false;
  }

  return true;
}

void Navigraph::lookup(float lonx, float laty, float r)
{
  log->println("Lookup...");
  nearest.clear();

  float a_lonx = lonx - r / 60.0;
  float b_lonx = lonx + r / 60.0;
  float a_laty = laty - r / 60.0;
  float b_laty = laty + r / 60.0;

  sprintf(sql, "SELECT ident, lonx, laty \
FROM waypoint \
WHERE \
lonx BETWEEN %f AND %f \
AND laty BETWEEN %f AND %f \
LIMIT 100",
          a_lonx, b_lonx, a_laty, b_laty);
  exec_nearest(NavWayPoint);

  sprintf(sql, "SELECT ident, lonx, laty \
FROM airport \
WHERE \
lonx BETWEEN %f AND %f \
AND laty BETWEEN %f AND %f \
LIMIT 100",
          a_lonx, b_lonx, a_laty, b_laty);
  exec_nearest(NavAirport);

  sprintf(sql, "SELECT ident, lonx, laty \
FROM vor \
WHERE \
lonx BETWEEN %f AND %f \
AND laty BETWEEN %f AND %f \
LIMIT 100",
          a_lonx, b_lonx, a_laty, b_laty);
  exec_nearest(NavVOR);

  log->println("Lookup done!");
}

void Navigraph::exec_nearest(NavPointType type)
{
  log->printf("Nearest: %s\n", sql);

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK)
  {
    log->printf("Failed to lookup: %s\n", sqlite3_errmsg(db));
    return;
  }

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
  {
    NearestPoint point;

    point.type = type;
    point.ident = std::string((const char *)sqlite3_column_text(stmt, 0));
    point.lonx = sqlite3_column_double(stmt, 1);
    point.laty = sqlite3_column_double(stmt, 2);

    nearest.push_back(point);

    log->println(point.ident.c_str());
  }
  sqlite3_finalize(stmt);

  log->println("Nearest done!");
}