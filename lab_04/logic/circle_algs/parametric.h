//
// Created by gregory on 24.03.2021.
//

#ifndef LAB_04_PARAMETRIC_H
#define LAB_04_PARAMETRIC_H

#include <vector>

#include <datatypes.h>

void paramcircle(std::vector<point_t> &dots, const point_t &c, double r,
                 const color_t &color);

void paramellipse(std::vector<point_t> &dots, const point_t &c, double ra,
                  double rb, const color_t &color);

#endif // LAB_04_PARAMETRIC_H
