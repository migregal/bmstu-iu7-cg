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
    if (display)
      drawer.draw_point(std::round(x), std::round(y), color);

    x += xsign;
    y += ysign;

    if (!steps)
      continue;

    if (std::round(x_buf) != std::round(x) &&
        std::round(y_buf) != std::round(y))
      ++step;

    x_buf = x;
    y_buf = y;
  }

  return step;
}
