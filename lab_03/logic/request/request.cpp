//
// Created by gregory on 15.03.2021.
//

#include <bresenham.h>
#include <cmath>
#include <dda.h>
#include <wu.h>

#include <request.h>

const char *alg_titles[ALGS_TITLES_LEN] = {
    "ЦДА", "Брезенхем (int)", "Брезенхем (float)", "Брезенхем (сглаживание)",
    "Ву",  "Library"};

request::request(drawer_mediator &mediator, const args &arg)
    : drawer(mediator), arg(arg) {}

int request::execute(const color_t &color, bool display) {
  switch (arg.command) {
  case DRAW_LINE:
    draw_line(color, display);
    return 0;
  case DRAW_BUNCH:
    draw_brunch(color, display);
    return 0;
  case CLEAR_SCREEN:
    drawer.clear();
    break;
  default:
    break;
  }
  return 0;
}

void request::draw_line(const color_t &color, bool display) {
  void (*func)(const line_t &, const color_t &, drawer_mediator &, bool);

  switch (arg.method) {
  case DDA:
    func = dda;
    break;
  case BRES_INT:
    func = bresenham_int;
    break;
  case BRES_FLOAT:
    func = bresenham_float;
    break;
  case BRES_ANIT:
    func = bresenham_antialised;
    break;
  case WU:
    func = wu;
    break;
  case STD:
    drawer.draw_line(arg.line.a, arg.line.b, color);
    return;
  default:
    return;
  }

  func(arg.line, color, drawer, display);
}

inline double to_rads(double degree) { return degree * M_PI / 180; }

void request::draw_brunch(const color_t &color, bool display) {
  auto center = arg.bunch.center;
  auto r = arg.bunch.r;

  void (*func)(const line_t &, const color_t &, drawer_mediator &, bool);

  switch (arg.method) {
  case DDA:
    func = dda;
    break;
  case BRES_INT:
    func = bresenham_int;
    break;
  case BRES_FLOAT:
    func = bresenham_float;
    break;
  case BRES_ANIT:
    func = bresenham_antialised;
    break;
  case WU:
    func = wu;
    break;
  case STD:
    for (auto i = 0; i <= 360; i += arg.bunch.step)
      drawer.draw_line(
          center,
          {center.x + r * cos(to_rads(i)), center.y + r * sin(to_rads(i))},
          color);
    return;
  default:
    return;
  }

  for (auto i = 0; i < 360; i += arg.bunch.step) {
    func({center,
          {center.x + r * cos(to_rads(i)), center.y + r * sin(to_rads(i))}},
         color, drawer, display);
  }
}
