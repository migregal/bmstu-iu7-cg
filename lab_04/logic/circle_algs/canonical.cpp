//
// Created by gregory on 24.03.2021.
//
#include <cmath>

#include <utils.h>

#include "canonical.h"

using namespace std;

void cancircle(std::vector<point_t> &dots, const point_t &center, double r,
               const color_t &color) {

  auto r_p = r * r;
  for (int32_t x = 0; x <= int(r / std::sqrt(2)); ++x) {
    auto y = sqrt(r_p - x * x);
    tmirrored(dots, {x + center.x, y + center.y}, center, color);
  }
}

void canellipse(std::vector<point_t> &dots, const point_t &c, double ra,
                double rb, const color_t &color) {

  auto ra_p = ra * ra, rb_p = rb * rb;
  auto limit = int(c.x + ra / std::sqrt(1 + rb_p / ra_p));

  for (int32_t x = c.x; x <= limit; ++x) {
    int32_t y = sqrt(ra_p * rb_p - (x - c.x) * (x - c.x) * rb_p) / ra + c.y;
    dmirrored(dots, {x, y}, c, color);
  }

  limit = int(c.y + rb / sqrt(1 + ra_p / rb_p));

  for (int32_t y = limit; y >= c.y; --y) {
    int32_t x = sqrt(ra_p * rb_p - (y - c.y) * (y - c.y) * ra_p) / rb + c.x;
    dmirrored(dots, {x, y}, c, color);
  }
}