//
// Created by gregory on 16.03.2021.
//

#include <cmath>
#include <dda.h>

void dda(const line_t &line, const color_t &color, std::vector<point_t> &data, bool display) {
  auto dx = line.b.x - line.a.x, dy = line.b.y - line.a.y;
  auto l = std::max(std::abs(dx), std::abs(dy));

  auto xsign = dx / l, ysign = dy/l;

  auto x = line.a.x, y = line.a.y;
  for (int i = 0; i < l; ++i) {
    auto rx = round(x), ry = round(y);
    if (display)
      data.emplace_back(rx, ry, color);

    x += xsign;
    y += ysign;
  }
}
