//
// Created by gregory on 15.03.2021.
//

#include <chrono>
#include <omp.h>

#include <bresenham.h>
#include <canonical.h>
#include <midpoint.h>
#include <parametric.h>

#include <request.h>

const char *alg_titles[ALGS_TITLES_LEN] = {
    "Каноническое ур-ие", "Параметрическое ур-ие", "Алглоритм Брезенхема",
    "Алгоритм ср. точки", "Library"};

using circle_func = void (*)(std::vector<point_t> &, const point_t &, double,
                             const color_t &);

using ellipse_func = void (*)(std::vector<point_t> &, const point_t &, double,
                              double, const color_t &);

request::request(drawer_mediator &mediator, const args &arg)
    : drawer(mediator), arg(arg) {}

void request::execute(const color_t &color, bool display) {
  switch (arg.command) {
  case DRAW_CIRCLE:
    draw_circle(color, display);
    break;
  case DRAW_CIRCLE_BUNCH:
    draw_circle_brunch(color, display);
    break;
  case DRAW_ELLIPSE:
    draw_ellipse(color, true);
    break;
  case DRAW_ELLIPSE_BUNCH:
    draw_ellipse_brunch(color, display);
    break;
  case MEASURE_CIRCLE_TIMES:
    measure_avg_circle_times(display);
    break;
  case MEASURE_ELLIPSE_TIMES:
    measure_avg_ellipse_times(display);
    break;
  case CLEAR_SCREEN:
    drawer.clear();
    break;
  default:
    break;
  }
}

inline circle_func get_circle_alg(int method) {
  switch (method) {
  case CANONICAL:
    return cancircle;
  case PARAMETRIC:
    return paramcircle;
  case BRESENHAM:
    return brescircle;
  case MID_POINT:
    return mpcircle;
  default:
    return nullptr;
  }
}

void request::draw_circle(const color_t &color, bool display) {
  if (STD == arg.method) {
    drawer.draw_circle(arg.circle.center, arg.circle.r, color);
    return;
  }

  auto func = get_circle_alg(arg.method);

  if (nullptr == func)
    return;

  std::vector<point_t> data;
  func(data, arg.circle.center, arg.circle.r, color);

  if (display)
    drawer.draw_points(data);
}

void request::draw_circle_brunch(const color_t &color, bool display) {
  auto b = arg.get_c_bunch();
  auto c = b.circle;

  if (STD == arg.method) {
    for (auto i = 0, step = 0; i < b.count; ++i, step += b.step)
      drawer.draw_circle(c.center, c.r + step, color);
    return;
  }

  auto func = get_circle_alg(arg.method);

  if (nullptr == func)
    return;

  std::vector<point_t> data;

  for (auto i = 0, step = 0; i < b.count; ++i, step += b.step)
    func(data, c.center, c.r + step, color);

  if (display)
    drawer.draw_points(data);
}

inline ellipse_func get_ellipse_alg(int method) {
  switch (method) {
  case CANONICAL:
    return canellipse;
  case PARAMETRIC:
    return paramellipse;
  case BRESENHAM:
    return bresellipse;
  case MID_POINT:
    return mpellipse;
  default:
    return nullptr;
  }
}

void request::draw_ellipse(const color_t &color, bool display) {
  if (STD == arg.method) {
    drawer.draw_ellipse(arg.ellipse.center, arg.ellipse.ra, arg.ellipse.rb,
                        color);
    return;
  }

  auto func = get_ellipse_alg(arg.method);

  if (nullptr == func)
    return;

  std::vector<point_t> data;
  func(data, arg.ellipse.center, arg.ellipse.ra, arg.ellipse.rb, color);

  if (display)
    drawer.draw_points(data);
}

void request::draw_ellipse_brunch(const color_t &color, bool display) {
  auto b = arg.get_e_bunch();
  auto e = b.ellipse;

  double ra = e.ra, rb = e.rb;
  auto koef = b.step_a ? rb / ra : ra / rb;

  if (STD == arg.method) {
    for (auto i = 0; i < b.count; ++i) {
      drawer.draw_ellipse(e.center, ra, rb, color);

      if (b.step_a) {
        ra += b.step_a;
        rb = ra * koef;
      } else {
        rb += b.step_b;
        ra = rb * koef;
      }
    }

    return;
  }

  auto func = get_ellipse_alg(arg.method);

  if (nullptr == func)
    return;

  std::vector<point_t> data;

  for (auto i = 0; i < b.count; ++i) {
    func(data, e.center, ra, rb, color);

    if (b.step_a) {
      ra += b.step_a;
      rb = ra * koef;
    } else {
      rb += b.step_b;
      ra = rb * koef;
    }
  }

  if (display)
    drawer.draw_points(data);
}

void request::measure_avg_circle_times(bool display) {
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::microseconds;

  auto count = 500;

  auto b = arg.get_m_c_bunch();
  auto c = b.get_circle();

  for (int i = CANONICAL; i < STD; ++i) {
    auto func = get_circle_alg(i);
    b.time->emplace_back();

    auto &cur_time = b.time->at(b.time->size() - 1);
    for (int j = 0, step = 0; j <= b.get_count(); ++j, step += b.get_step()) {
      auto res = 0.;

      for (int k = 0; k < count; ++k) {
        auto data = std::vector<point_t>();
        auto t = omp_get_wtime();
        func(data, c.center, c.r + step, {});

        if (display)
          drawer.draw_points(data);

        t = omp_get_wtime() - t;
        res += t;
      }

      cur_time.emplace_back(c.r + step, res / count * 1e6);
    }
  }

  b.time->emplace_back();

  auto end = high_resolution_clock::now();
  auto start = high_resolution_clock::now();

  auto lib_count = 100;

  auto &cur_time = b.time->at(b.time->size() - 1);
  for (auto i = 0, step = 0; i <= b.get_count(); ++i, step += b.get_step()) {
    auto res = 0.;

    for (int j = 0; j < lib_count; ++j) {
      start = high_resolution_clock::now();
      drawer.draw_circle(c.center, c.r + step, {});
      end = high_resolution_clock::now();

      drawer.clear();
      res += duration_cast<microseconds>(end - start).count();
    }

    cur_time.emplace_back(c.r + step, res / lib_count);
  }

  drawer.clear();
}

void request::measure_avg_ellipse_times(bool display) {
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::microseconds;

  auto count = 1000;

  auto b = arg.get_m_e_bunch();
  auto c = b.get_ellipse();

  for (int i = CANONICAL; i < STD; ++i) {
    auto func = get_ellipse_alg(i);
    b.time->emplace_back();

    auto &cur_time = b.time->at(b.time->size() - 1);
    for (int j = 0, step = 0; j <= b.get_count(); ++j, step += b.get_step_a()) {
      auto res = 0.;

      for (int k = 0; k < count; ++k) {
        auto data = std::vector<point_t>();
        auto t = omp_get_wtime();
        func(data, c.center, c.ra + step, c.rb + step, {});

        if (display)
          drawer.draw_points(data);

        t = omp_get_wtime() - t;
        res += t;
      }

      cur_time.emplace_back(c.ra + step, res / count * 1e6);
    }
  }

  b.time->emplace_back();

  auto end = high_resolution_clock::now();
  auto start = high_resolution_clock::now();

  auto lib_count = 100;

  auto &cur_time = b.time->at(b.time->size() - 1);
  for (auto i = 0, step = 0; i <= b.get_count(); ++i, step += b.get_step_a()) {
    auto res = 0.;

    for (int j = 0; j < lib_count; ++j) {
      start = high_resolution_clock::now();
      drawer.draw_ellipse(c.center, c.ra + step, c.rb + step, {});
      end = high_resolution_clock::now();

      drawer.clear();
      res += duration_cast<microseconds>(end - start).count();
    }

    cur_time.emplace_back(c.ra + step, res / lib_count);
  }

  drawer.clear();
}
