//
// Created by gregory on 24.03.2021.
//

#include <utils.h>

#include "midpoint.h"

void mpcircle(std::vector<point_t> &dots, const point_t &c, double r,
              const color_t &color) {
  auto x = r, y = 0.;

  tmirrored(dots, {x + c.x, y + c.y}, c, color);
  auto delta = 1 - r;

  while (x > y) {
    ++y;

    if (delta > 0) {
      --x;
      delta -= 2 * x - 2;
    }

    delta += 2 * y + 3;

    tmirrored(dots, {x + c.x, y + c.y}, c, color);
  }
}

void mpellipse(std::vector<point_t> &dots, const point_t &c, double ra,
               double rb, const color_t &color) {
  auto x = 0., y = rb;

  auto delta = rb * rb - ra * ra * rb + 0.25 * ra * ra;
  auto dx = 2 * rb * rb * x, dy = 2 * ra * ra * y;

  while (dx < dy) {
    dmirrored(dots, {x + c.x, y + c.y}, c, color);
    ++x;
    dx += 2 * rb * rb;

    if (delta >= 0) {
      --y;
      dy -= 2 * ra * ra;
      delta -= dy;
    }

    delta += dx + rb * rb;
  }

  delta = rb * rb * (x + 0.5) * (x + 0.5) + ra * ra * (y - 1) * (y - 1) -
          ra * ra * rb * rb;

  while (y >= 0) {
    dmirrored(dots, {x + c.x, y + c.y}, c, color);

    --y;
    dy -= 2 * ra * ra;

    if (delta <= 0) {
      ++x;
      dx += 2 * rb * rb;
      delta += dx;
    }

    delta -= dy - ra * ra;
  }
}