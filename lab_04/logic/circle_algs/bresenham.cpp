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
    if (delta <= 0) {
      auto temp = 2 * (delta + y) - 1;
      ++x;
      if (temp >= 0) {
        delta += 2 * (x - y + 1);
        --y;
      } else
        delta += 2 * x + 1;
    }

    else {
      auto temp = 2 * (delta - x) - 1;
      --y;
      if (temp < 0) {
        delta += 2 * (x - y + 1);
        ++x;
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

  dmirrored(dots, {x + c.x, y + c.y}, c, color);

  while (y > 0) {
    if (delta <= 0) {
      auto temp = 2 * delta + ra * ra * (2 * y - 1);
      ++x;
      delta += rb * rb * (2 * x + 1);
      if (temp >= 0) {
        --y;
        delta += ra * ra * (-2 * y + 1);
      }
    } else {
      auto temp = 2 * delta + rb * rb * (-2 * x - 1);
      --y;
      delta += ra * ra * (-2 * y + 1);
      if (temp < 0) {
        ++x;
        delta += rb * rb * (2 * x + 1);
      }
    }

    dmirrored(dots, {x + c.x, y + c.y}, c, color);
  }
}
