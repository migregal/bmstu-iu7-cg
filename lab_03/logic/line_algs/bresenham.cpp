//
// Created by gregory on 16.03.2021.
//

#include <cmath>

#include <bresenham.h>
#include <utils.h>

int32_t bresenham_int(const line_t &line, const color_t &color,
                      drawer_mediator &drawer, bool display, bool steps) {
  int32_t dx = line.b.x - line.a.x, dy = line.b.y - line.a.y;

  auto xsign = sign(dx), ysign = sign(dy);

  dx = std::abs(dx), dy = std::abs(dy);

  auto change = 0;
  if (dy >= dx) {
    std::swap(dx, dy);
    change = 1;
  }

  auto m = 2 * dy, m1 = 2 * dx;
  auto e = m - dx;

  int32_t x = round(line.a.x), y = round(line.a.y);
  int32_t step = 1, x_buf = x, y_buf = y;

  for (auto i = 0; i <= dx; ++i) {
    if (display)
      drawer.draw_point(x, y, color);

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

    if(!steps)
      continue;

    if (!((x_buf == x && y_buf != y) || (x_buf != x && y_buf == y)))
      ++step;

    x_buf = x;
    y_buf = y;
  }

  return step;
}

int32_t bresenham_float(const line_t &line, const color_t &color,
                        drawer_mediator &drawer, bool display, bool steps) {
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
  auto x_buf = x, y_buf = y;
  int32_t step = 1;

  for (auto i = 0; i <= dx; ++i) {
    if (display)
      drawer.draw_point(x, y, color);

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

    if(!steps)
      continue;

    if (!((x_buf == x && y_buf != y) || (x_buf != x && y_buf == y)))
      ++step;

    x_buf = x;
    y_buf = y;
  }

  return step;
}

int32_t bresenham_antialised(const line_t &line, const color_t &color,
                             drawer_mediator &drawer, bool display,
                             bool steps) {
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

  color_t color1 = update(color, e);
  auto x = round(line.a.x), y = round(line.a.y);
  if (display)
    drawer.draw_point(x, y, color1);

  auto W = 1 - m;
  auto x_buf = x, y_buf = y;
  int32_t step = 1;

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

    color1 = update(color, e);
    if (display)
      drawer.draw_point(x, y, color1);

    if(!steps)
      continue;

    if (!((x_buf == x && y_buf != y) || (x_buf != x && y_buf == y)))
      ++step;

    x_buf = x;
    y_buf = y;
  }

  return step;
}