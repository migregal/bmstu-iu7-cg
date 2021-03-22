//
// Created by gregory on 04.03.2021.
//
#include "drawer_mediator.h"

drawer_mediator::drawer_mediator(QGraphicsScene *scene) : scene(scene) {}

void drawer_mediator::draw_line(const point_t &p1, const point_t &p2,
                                const color_t color) {
  scene->addLine(p1.x, p1.y, p2.x, p2.y,
                 {qRgba(color.r, color.g, color.b, color.alpha)});
}

void drawer_mediator::draw_point(double x, double y, const color_t &color) {
  scene->addLine(x, y, x + 0.5, y, {{color.r, color.g, color.b, color.alpha}});
}

void drawer_mediator::clear() { scene->clear(); }
