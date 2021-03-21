//
// Created by gregory on 16.03.2021.
//

#include <cmath>

#include <utils.h>
#include <bresenham.h>

void bresenham_int(const line_t &line, const color_t &color,
                   std::vector<point_t> &data) {
  auto dx = line.b.x - line.a.x, dy = line.b.y - line.a.y;

  auto xsign = sign(dx), ysign = sign(dy);

  dx = std::abs(dx), dy = std::abs(dy);

  auto change = 0;
  if (dy >= dx) {
    std::swap(dx, dy);
    change = 1;
  }

  auto m = 2 * dy, m1 = 2 * dx;
  auto e = m - dx;

  auto x = round(line.a.x), y = round(line.a.y);
  for (auto i = 0; i <= dx; ++i) {
      data.emplace_back(x, y, color);

    if (e >= 0) {
      if (1 == change)
        x += xsign;
      else
        y += ysign;
      e -= m1;
    }
    if (e <= 0) {
      if (1 == change)
        y += ysign;
      else
        x += xsign;
      e += m;
    }
  }
}

void bresenham_float(const line_t &line, const color_t &color,
                     std::vector<point_t> &data) {
  auto dx = line.b.x - line.a.x, dy = line.b.y - line.a.y;
  int32_t xsign = sign(dx), ysign = sign(dy);

  dx = std::abs(dx), dy = std::abs(dy);

  auto change = 0;
  if (dy >= dx) {
    std::swap(dx, dy);
    change = 1;
  }

  auto m = dy / dx;
  auto e = m - 0.5;

  auto x = round(line.a.x), y = round(line.a.y);
  for (auto i = 0; i <= dx; ++i) {
      data.emplace_back(x, y, color);

    if (e >= 0) {
      if (1 == change)
        x += xsign;
      else
        y += ysign;
      --e;
    }
    if (e <= 0) {
      if (1 == change)
        y += ysign;
      else
        x += xsign;
      e += m;
    }
  }
}

void bresenham_antialised(const line_t &line, const color_t &color,
                          std::vector<point_t> &data) {
  auto dx = line.b.x - line.a.x, dy = line.b.y - line.a.y;
  auto xsign = sign(dx), ysign = sign(dy);

  dx = std::abs(dx), dy = std::abs(dy);

  auto change = 0;
  if (dy >= dx) {
    std::swap(dx, dy);
    change = 1;
  }

  auto m = dy / dx;
  auto e = 0.5;

  auto x = round(line.a.x), y = round(line.a.y);
  data.emplace_back(x, y, update(color, e));

  auto W = 1 - m;
  for (int i = 0; i <= dx; ++i) {
    if (e < W) {
      if (0 == change)
        x += xsign;
      else
        y += ysign;
      e += m;
    } else {
      x += xsign;
      y += ysign;
      e -= W;
    }

    data.emplace_back(x, y, update(color, e));
  }
}