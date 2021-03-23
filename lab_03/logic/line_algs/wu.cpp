//
// Created by gregory on 16.03.2021.
//

#include <cinttypes>
#include <cmath>

#include <QDebug>
#include <utils.h>
#include <wu.h>

int32_t wu(const line_t &line, const color_t &color, drawer_mediator &drawer,
           bool display, bool steps) {
  auto a = line.a, b = line.b;

  auto ipart = [](double x) -> int { return int(std::floor(x)); };
  auto round = [](double x) -> double { return std::round(x); };
  auto fpart = [](double x) -> double { return x - std::floor(x); };
  auto rfpart = [=](double x) -> double { return 1 - fpart(x); };

  color_t color1, color2;

  const bool steep = std::abs(b.y - a.y) > std::abs(b.x - a.x);
  if (steep) {
    std::swap(a.x, a.y);
    std::swap(b.x, b.y);
  }
  if (a.x > b.x) {
    std::swap(a.x, b.x);
    std::swap(a.y, b.y);
  }

  const auto dx = b.x - a.x;
  const auto dy = b.y - a.y;
  const double gradient = (dx == 0) ? 1 : dy / dx;

  int xpx11;
  double intery;
  {
    const double xend = round(a.x);
    const double yend = a.y + gradient * (xend - a.x);
    const double xgap = rfpart(a.x + 0.5);
    xpx11 = int(xend);
    const int ypx11 = ipart(yend);

    color1 = update(color, rfpart(yend) * xgap);
    color2 = update(color, fpart(yend) * xgap);
    if (display) {
      if (steep) {
        drawer.draw_point(ypx11, xpx11, color1);
        drawer.draw_point(ypx11 + 1, xpx11, color2);
      } else {
        drawer.draw_point(xpx11, ypx11, color1);
        drawer.draw_point(xpx11, ypx11 + 1, color2);
      }
    }

    intery = yend + gradient;
  }

  int xpx12;
  {
    const double xend = std::round(b.x);
    const double yend = b.y + gradient * (xend - b.x);
    const double xgap = rfpart(b.x + 0.5);
    xpx12 = int(xend);
    const int ypx12 = ipart(yend);

    color1 = update(color, rfpart(yend) * xgap);
    color2 = update(color, fpart(yend) * xgap);
    if (display) {
      if (steep) {
        drawer.draw_point(ypx12, xpx12, color1);
        drawer.draw_point(ypx12 + 1, xpx12, color2);
      } else {
        drawer.draw_point(xpx12, ypx12, color1);
        drawer.draw_point(xpx12, ypx12 + 1, color2);
      }
    }
  }

  auto step = 0;

  for (int x = xpx11 + 1; x < xpx12; x++) {
    color1 = update(color, rfpart(intery));
    color2 = update(color, fpart(intery));

    if (display) {
      if (steep) {
        drawer.draw_point(ipart(intery), x, color1);
        drawer.draw_point(ipart(intery) + 1, x, color2);
      } else {
        drawer.draw_point(x, ipart(intery), color1);
        drawer.draw_point(x, ipart(intery) + 1, color2);
      }
    }

    if (steps && x < xpx12) {
      if (ipart(intery) != ipart(intery + gradient))
        ++step;
    }

    intery += gradient;
  }

  return step;
}
