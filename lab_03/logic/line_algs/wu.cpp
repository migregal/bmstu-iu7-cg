//
// Created by gregory on 16.03.2021.
//

#include <cinttypes>
#include <cmath>

#include <utils.h>
#include <wu.h>

void wu(const line_t &line, const color_t &color, std::vector<point_t> &data) {
  auto a = line.a, b = line.b;

  auto ipart = [](double x) -> int { return int(std::floor(x)); };
  auto round = [](double x) -> double { return std::round(x); };
  auto fpart = [](double x) -> double { return x - std::floor(x); };
  auto rfpart = [=](double x) -> double { return 1 - fpart(x); };

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
      if (steep) {
        data.emplace_back(ypx11, xpx11, update(color, rfpart(yend) * xgap));
        data.emplace_back(ypx11 + 1, xpx11, update(color, fpart(yend) * xgap));
      } else {
        data.emplace_back(xpx11, ypx11, update(color, rfpart(yend) * xgap));
        data.emplace_back(xpx11, ypx11 + 1, update(color, fpart(yend) * xgap));
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
      if (steep) {
        data.emplace_back(ypx12, xpx12, update(color, rfpart(yend) * xgap));
        data.emplace_back(ypx12 + 1, xpx12, update(color, fpart(yend) * xgap));
      } else {
        data.emplace_back(xpx12, ypx12, update(color, rfpart(yend) * xgap));
        data.emplace_back(xpx12, ypx12 + 1, update(color, fpart(yend) * xgap));
      }

  }

  if (steep) {
    for (int x = xpx11 + 1; x < xpx12; x++) {
      data.emplace_back(ipart(intery), x, update(color, rfpart(intery)));
      data.emplace_back(ipart(intery) + 1, x, update(color, fpart(intery)));

      intery += gradient;
    }
    return;
  }

  for (int x = xpx11 + 1; x < xpx12; x++) {
    data.emplace_back(x, ipart(intery), update(color, rfpart(intery)));
    data.emplace_back(x, ipart(intery) + 1, update(color, fpart(intery)));
    intery += gradient;
  }
}
