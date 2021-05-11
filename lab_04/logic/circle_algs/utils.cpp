//
// Created by gregory on 24.03.2021.
//

#include <vector>

#include <datatypes.h>

void tmirrored(std::vector<point_t> &dots, point_t point, const point_t &c,
               color_t color) {

  dots.emplace_back(point.x, point.y, color);
  dots.emplace_back(2 * c.x - point.x, point.y, color);
  dots.emplace_back(point.x, 2 * c.y - point.y, color);
  dots.emplace_back(2 * c.x - point.x, 2 * c.y - point.y, color);
  dots.emplace_back(point.y + c.x - c.y,
                    point.x + c.y - c.x, color);
  dots.emplace_back(-point.y + c.x + c.y,
                    point.x + c.y - c.x, color);
  dots.emplace_back(point.y + c.x - c.y,
                    -point.x + c.y + c.x, color);
  dots.emplace_back(-point.y + c.x + c.y,
                    -point.x + c.x + c.y, color);
}

void dmirrored(std::vector<point_t> &dots, point_t point, const point_t &center,
               color_t color) {

  dots.emplace_back(point.x, point.y, color);
  dots.emplace_back(2 * center.x - point.x, point.y, color);
  dots.emplace_back(point.x, 2 * center.y - point.y, color);
  dots.emplace_back(2 * center.x - point.x, 2 * center.y - point.y, color);
}