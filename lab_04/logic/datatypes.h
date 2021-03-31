//
// Created by gregory on 16.03.2021.
//

#ifndef LAB_03_DATATYPES_H
#define LAB_03_DATATYPES_H

#include <cstdint>
#include <vector>

using timed_vect = std::vector<std::pair<double, double>>;

struct color_t {
  int r = 0, g = 0, b = 0;
  int alpha = 255;
};

struct point_t {
  double x = 0, y = 0;
  color_t color;

  point_t() = default;
  point_t(double x_, double y_) : x(x_), y(y_) {}
  point_t(double x_, double y_, color_t color_) : x(x_), y(y_), color(color_) {}
};

struct circle_t {
  point_t center = {};
  int32_t r = 0;
};

struct circle_bunch_t {
  circle_t circle{};
  int32_t count = 1;
  int32_t step = 1;
};

struct ellipse_t {
  point_t center = {};
  int32_t ra = 0, rb = 0;
};

struct ellipse_bunch_t {
  ellipse_t ellipse{};
  int32_t count = 1;
  int32_t step = 1;
};

struct measure_circle_bunch_t {
  circle_bunch_t bunch;
  std::vector<timed_vect> *time = nullptr;

  [[nodiscard]] inline circle_t get_circle() const { return bunch.circle; }

  [[nodiscard]] inline int32_t get_step() const { return bunch.step; }

  [[nodiscard]] inline int32_t get_count() const { return bunch.count; }
};

struct measure_ellipse_bunch_t {
  ellipse_bunch_t bunch;
  std::vector<double> *time = nullptr;

  [[nodiscard]] inline ellipse_t get_ellipse() const { return bunch.ellipse; }

  [[nodiscard]] inline int32_t get_step() const { return bunch.step; }

  [[nodiscard]] inline int32_t get_count() const { return bunch.count; }
};

#endif // LAB_03_DATATYPES_H
