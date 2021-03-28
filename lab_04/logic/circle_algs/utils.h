//
// Created by gregory on 24.03.2021.
//

#ifndef LAB_04_UTILS_H
#define LAB_04_UTILS_H

#include <vector>

#include <datatypes.h>

void tmirrored(std::vector<point_t> &dots, point_t point, const point_t &c,
               color_t color);

void dmirrored(std::vector<point_t> &dots, point_t point, const point_t &center,
               color_t color);

#endif // LAB_04_UTILS_H
