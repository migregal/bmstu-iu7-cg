//
// Created by gregory on 04.03.2021.
//
#include "drawer_mediator.h"

drawer_mediator::drawer_mediator(QGraphicsScene *scene) : scene(scene) {}

void drawer_mediator::draw_line(const point_t &p1, const point_t &p2,
                                const color_t color) {
  scene->addLine(p1.x, p1.y, p2.x, p2.y, {qRgb(color.r, color.g, color.b)});
}

void drawer_mediator::draw_points(std::vector<point_t> &points) {
  for (const auto &p : points)
    scene->addLine(p.x, p.y, p.x, p.y, {qRgb(p.color.r, p.color.g, p.color.b)});
}

void drawer_mediator::clear() { scene->clear(); }
