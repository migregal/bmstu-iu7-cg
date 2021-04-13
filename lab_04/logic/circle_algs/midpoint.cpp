//
// Created by gregory on 24.03.2021.
//

#include <utils.h>

#include "midpoint.h"

void mpcircle(std::vector<point_t> &dots, const point_t &c, double r,
              const color_t &color) {
  auto x = r, y = 0.;

  auto delta = 1 - r;

  while (x > y) {
    tmirrored(dots, {x + c.x, y + c.y}, c, color);
    ++y;

    if (delta > 0) {
      --x;
      delta -= 2 * x - 2;
    }

    delta += 2 * y + 3;
  }
}

void mpellipse(std::vector<point_t> &dots, const point_t &c, double ra,
               double rb, const color_t &color) {
  auto x = 0., y = rb;

  auto pa = ra * ra, pb = rb * rb;

  auto delta = pb - pa * rb + 0.25 * pa;
  auto dx = 2 * pb * x, dy = 2 * pa * y;

  while (dx < dy) {
    dmirrored(dots, {x + c.x, y + c.y}, c, color);
    ++x;
    dx += 2 * rb * rb;

    if (delta >= 0) {
      --y;
      dy -= 2 * pa;
      delta -= dy;
    }

    delta += dx + pb;
  }

  delta = pb * (x + 0.5) * (x + 0.5) + pa * (y - 1) * (y - 1) - pa * pb;

  while (y >= 0) {
    dmirrored(dots, {x + c.x, y + c.y}, c, color);

    --y;
    dy -= 2 * pa;

    if (delta <= 0) {
      ++x;
      dx += 2 * pb;
      delta += dx;
    }

    delta -= dy - pa;
  }
}