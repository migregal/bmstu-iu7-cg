//
// Created by gregory on 24.03.2021.
//

#include <utils.h>

#include "bresenham.h"

void brescircle(std::vector<point_t> &dots, const point_t &c, double r,
                const color_t &color) {
  auto x = 0., y = r;
  auto delta = 2 * (1 - r);

  tmirrored(dots, {x + c.x, y + c.y}, c, color);

  while (x < y) {
    if (delta < 0) {
      auto temp = 2 * (delta + y) - 1;
      ++x;
      if (temp > 0) {
        --y;
        delta += 2 * (x - y + 1);
      } else
        delta += 2 * x + 1;
    } else {
      auto temp = 2 * (delta - x) - 1;
      --y;
      if (temp < 0) {
        ++x;
        delta += 2 * (x - y + 1);
      } else
        delta -= 2 * y - 1;
    }

    tmirrored(dots, {x + c.x, y + c.y}, c, color);
  }
}

void bresellipse(std::vector<point_t> &dots, const point_t &c, double ra,
                 double rb, const color_t &color) {
  auto x = 0., y = rb;

  auto delta = rb * rb - ra * ra * (2 * rb + 1);

  auto pa = ra * ra, pb = rb * rb;

  while (y >= 0) {
    dmirrored(dots, {x + c.x, y + c.y}, c, color);

    if (delta < 0) {
      auto temp = 2 * delta + pa * (2 * y - 1);
      ++x;
      delta += pb * (2 * x + 1);
      if (temp > 0) {
        --y;
        delta += pa * (-2 * y + 1);
      }
    } else if (0 == delta) {
      ++x;
      --y;
      delta += pb * (2 * x + 1) + (1 - 2 * y) * pa;
    } else {
      auto temp = 2 * delta + pb * (-2 * x - 1);
      --y;
      delta += pa * (-2 * y + 1);
      if (temp < 0) {
        ++x;
        delta += pb * (2 * x + 1);
      }
    }
  }
}
