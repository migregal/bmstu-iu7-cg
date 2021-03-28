//
// Created by gregory on 04.03.2021.
//
#include "drawer_mediator.h"

drawer_mediator::drawer_mediator(QGraphicsScene *scene) : scene(scene) {}

void drawer_mediator::draw_circle(const point_t &c, const double r,
                                  const color_t &color) {
  scene->addEllipse(c.x - r, c.y - r, 2 * r, 2 * r,
                    {qRgba(color.r, color.g, color.b, color.alpha)});
}

void drawer_mediator::draw_ellipse(const point_t &c, const double ra,
                                   const double rb, const color_t &color) {
  scene->addEllipse(c.x - ra, c.y - rb, 2 * ra, 2 * rb,
                    {qRgba(color.r, color.g, color.b, color.alpha)});
}

void drawer_mediator::draw_point(double x, double y, const color_t &color) {
  scene->addLine(x, y, x, y, {{color.r, color.g, color.b, color.alpha}});
}

void drawer_mediator::draw_points(std::vector<point_t> &points) {
  for (auto &p : points)
    draw_point(p.x, p.y, p.color);
}

void drawer_mediator::clear() { scene->clear(); }
