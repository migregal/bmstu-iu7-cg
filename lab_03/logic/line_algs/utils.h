//
// Created by gregory on 17.03.2021.
//

#ifndef LAB_03_UTILS_H
#define LAB_03_UTILS_H

#include <datatypes.h>

int clamp_255(int n);

color_t update(const color_t &color, double intensivity);

inline int sign(double x) {
  if (x < 0)
    return -1;

  if (x > 0)
    return 1;

  return 0;
}

#endif // LAB_03_UTILS_H
