//
// Created by gregory on 17.03.2021.
//

#include <utils.h>

int clamp_255(const int n) {
  int x = n > 255 ? 255 : n;
  return x < 0 ? 0 : x;
}

color_t update(const color_t &color, double e) {
  return {color.r, color.g, color.b, clamp_255(int(255 * e))};
}