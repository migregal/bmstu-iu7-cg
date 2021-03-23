//
// Created by gregory on 15.03.2021.
//

#include <chrono>
#include <cmath>

#include <bresenham.h>
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
  case DRAW_LINE_STEP_COUNT:
    return draw_line(color, display, true);
  case DRAW_BUNCH:
    draw_brunch(color, display);
    return 0;
  case MEASURE_TIMES:
    measure_avg_times(display);
    break;
  case CLEAR_SCREEN:
    drawer.clear();
    break;
  default:
    break;
  }
  return 0;
}

int request::draw_line(const color_t &color, bool display, bool steps) {
  int32_t (*func)(const line_t &, const color_t &, drawer_mediator &, bool,
                  bool);

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
    return 0;
  default:
    return -1;
  }

  return func(arg.line, color, drawer, display, steps);
}

inline double to_rads(double degree) { return degree * M_PI / 180; }

void request::draw_brunch(const color_t &color, bool display) {
  auto center = arg.bunch.center;
  auto r = arg.bunch.r;

  int32_t (*func)(const line_t &, const color_t &, drawer_mediator &, bool,
                  bool);

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
         color, drawer, display, false);
  }
}

void request::measure_avg_times(bool display) {
  auto count = 1000;
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::microseconds;

  auto end = high_resolution_clock::now();
  auto start = high_resolution_clock::now();

  for (int i = 0; i < count; ++i)
    for (auto t = 0; t < 360; t += arg.m_bunch.bunch.step) {
      dda({{},
           {arg.m_bunch.bunch.r * cos(to_rads(t)),
            arg.m_bunch.bunch.r * sin(to_rads(t))}},
          {}, drawer, display, false);
    }
  end = high_resolution_clock::now();

  arg.m_bunch.time->push_back(
      (double)duration_cast<microseconds>(end - start).count() / count);

  start = high_resolution_clock::now();

  for (int i = 0; i < count; ++i)
    for (auto t = 0; t < 360; t += arg.m_bunch.bunch.step) {
      bresenham_int({{},
                     {arg.m_bunch.bunch.r * cos(to_rads(t)),
                      arg.m_bunch.bunch.r * sin(to_rads(t))}},
                    {}, drawer, display, false);
    }
  end = high_resolution_clock::now();

  arg.m_bunch.time->push_back(
      (double)duration_cast<microseconds>(end - start).count() / count);

  start = high_resolution_clock::now();

  for (int i = 0; i < count; ++i)
    for (auto t = 0; t < 360; t += arg.m_bunch.bunch.step) {
      bresenham_float({{},
                       {arg.m_bunch.bunch.r * cos(to_rads(t)),
                        arg.m_bunch.bunch.r * sin(to_rads(t))}},
                      {}, drawer, display, false);
    }
  end = high_resolution_clock::now();

  arg.m_bunch.time->push_back(
      (double)duration_cast<microseconds>(end - start).count() / count);

  start = high_resolution_clock::now();

  for (int i = 0; i < count; ++i)
    for (auto t = 0; t < 360; t += arg.m_bunch.bunch.step) {
      bresenham_antialised({{},
                            {arg.m_bunch.bunch.r * cos(to_rads(t)),
                             arg.m_bunch.bunch.r * sin(to_rads(t))}},
                           {}, drawer, display, false);
    }
  end = high_resolution_clock::now();

  arg.m_bunch.time->push_back(
      (double)duration_cast<microseconds>(end - start).count() / count);

  start = high_resolution_clock::now();

  for (int i = 0; i < count; ++i)
    for (auto t = 0; t < 360; t += arg.m_bunch.bunch.step) {
      wu({{},
          {arg.m_bunch.bunch.r * cos(to_rads(t)),
           arg.m_bunch.bunch.r * sin(to_rads(t))}},
         {}, drawer, display, false);
    }
  end = high_resolution_clock::now();

  arg.m_bunch.time->push_back(
      (double)duration_cast<microseconds>(end - start).count() / count);

  start = high_resolution_clock::now();

  for (int i = 0; i < count; ++i)
    for (auto t = 0; t < 360; t += arg.m_bunch.bunch.step) {
      drawer.draw_line({},
                       {arg.m_bunch.bunch.r * cos(to_rads(t)),
                        arg.m_bunch.bunch.r * sin(to_rads(t))},
                       {});
    }
  end = high_resolution_clock::now();

  arg.m_bunch.time->push_back(
      (double)duration_cast<microseconds>(end - start).count() / count);

  drawer.clear();
}