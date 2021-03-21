//
// Created by gregory on 16.03.2021.
//

#ifndef LAB_03_DATATYPES_H
#define LAB_03_DATATYPES_H

#include <cstdint>

struct color_t {
  int r = 0, g = 0, b = 0;
//  int opacity = 255;
};

struct point_t {
  double x = 0, y = 0;
  color_t color;

  point_t() = default;
  point_t(double x_, double y_) : x(x_), y(y_) {}
  point_t(double x_, double y_, color_t c_) : x(x_), y(y_), color(c_) {}
};

struct line_t {
  point_t a, b;
};

struct bunch_t {
  point_t center;
  uint32_t r = 1;
  uint32_t step = 1;
};

#endif // LAB_03_DATATYPES_H
