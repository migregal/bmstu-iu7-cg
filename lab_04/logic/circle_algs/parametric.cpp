//
// Created by gregory on 24.03.2021.
//

#include <cmath>

#include <utils.h>

#include <parametric.h>

void paramcircle(std::vector<point_t> &dots, const point_t &c, double r,
                 const color_t &color) {
  auto step = 1 / r;

  for (auto t = 0.; t <= M_PI_4; t += step) {
    tmirrored(dots, {c.x + r * cos(t), c.y + r * sin(t)}, c, color);
  }
}

void paramellipse(std::vector<point_t> &dots, const point_t &c, double ra,
                  double rb, const color_t &color) {
  auto step = ra > rb ? 1 / ra : 1 / rb;

  for (auto t = 0.; t <= M_PI_2; t += step) {
    dmirrored(dots, {c.x + ra * cos(t), c.y + rb * sin(t)}, c, color);
  }
}