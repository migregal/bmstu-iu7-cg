//
// Created by gregory on 24.03.2021.
//
#include <cmath>

#include <utils.h>

#include "canonical.h"

using namespace std;

void cancircle(std::vector<point_t> &dots, const point_t &center, double r,
               const color_t &color) {
  auto limit = int32_t(r / std::sqrt(2));

  auto pr = r * r;
  for (int32_t x = 0; x <= limit; ++x) {
    auto y = int32_t(sqrt(pr - x * x));
    tmirrored(dots, {x + center.x, y + center.y}, center, color);
  }
}

void canellipse(std::vector<point_t> &dots, const point_t &c, double ra,
                double rb, const color_t &color) {
  auto pa = ra * ra, pb = rb * rb;

  auto limit = int32_t(c.x + pa / std::sqrt(pa + pb));
  for (int32_t x = c.x; x <= limit; ++x) {
    int32_t y = round(sqrt(1 - x * x / pa) * rb) + c.y;
    dmirrored(dots, {x, y}, c, color);
  }

  limit = int(c.y + pb / sqrt(pa + pb));
  for (int32_t y = limit; y >= c.y; --y) {
    int32_t x =
        round(sqrt(1 - y * y) / rb) + c.x;
    dmirrored(dots, {x, y}, c, color);
  }
}