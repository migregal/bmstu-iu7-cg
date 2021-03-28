//
// Created by gregory on 24.03.2021.
//

#ifndef LAB_04_MIDPOINT_H
#define LAB_04_MIDPOINT_H


#include <vector>

#include <datatypes.h>

void mpcircle(std::vector<point_t> &dots, const point_t &c, double r,
                const color_t &color);

void mpellipse(std::vector<point_t> &dots, const point_t &c, double ra,
                 double rb, const color_t &color);

#endif // LAB_04_MIDPOINT_H
