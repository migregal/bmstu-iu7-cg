//
// Created by gregory on 16.03.2021.
//

#ifndef LAB_03_BRESENHAM_H
#define LAB_03_BRESENHAM_H

#include <drawer_mediator.h>

void bresenham_int(const line_t &line, const color_t &color,
                   drawer_mediator &drawer, bool display);

void bresenham_float(const line_t &line, const color_t &color,
                     drawer_mediator &drawer, bool display);

void bresenham_antialised(const line_t &line, const color_t &color,
                          drawer_mediator &drawer, bool display);

#endif // LAB_03_BRESENHAM_H
