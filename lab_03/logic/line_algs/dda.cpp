//
// Created by gregory on 16.03.2021.
//

#include <cmath>
#include <dda.h>

int32_t dda(const line_t &line, const color_t &color, drawer_mediator &drawer,
            bool display, bool steps) {
  auto dx = line.b.x - line.a.x, dy = line.b.y - line.a.y;
  auto l = std::max(std::abs(dx), std::abs(dy));

  auto xsign = dx / l, ysign = dy / l;

  auto x = line.a.x, y = line.a.y;
  auto x_buf = x, y_buf = y;
  int32_t step = 1;

  for (int i = 0; i < l; ++i) {
    auto rx = round(x), ry = round(y);
    if (display)
      drawer.draw_point(rx, ry, color);

    x += xsign;
    y += ysign;

    if (!steps)
      continue;

    if (!((round(x_buf) == round(x) && round(y_buf) != round(y)) ||
          (round(x_buf) != round(x) && round(y_buf) == round(y))))
      ++step;

    x_buf = x;
    y_buf = y;
  }

  return step;
}
