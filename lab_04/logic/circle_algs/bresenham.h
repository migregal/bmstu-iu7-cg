//
// Created by gregory on 24.03.2021.
//

#ifndef LAB_04_BRESENHAM_H
#define LAB_04_BRESENHAM_H

#include <vector>

#include <datatypes.h>

void brescircle(std::vector<point_t> &dots, const point_t &c, double r,
                 const color_t &color);

void bresellipse(std::vector<point_t> &dots, const point_t &c, double ra,
                  double rb, const color_t &color);

#endif // LAB_04_BRESENHAM_H
