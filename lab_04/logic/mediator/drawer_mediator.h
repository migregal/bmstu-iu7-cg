//
// Created by gregory on 03.03.2021.
//

#ifndef LAB_04_DRAWER_MEDIATOR_H
#define LAB_04_DRAWER_MEDIATOR_H

#include <QGraphicsScene>
#include <datatypes.h>

enum { BLACK, WHITE };

class drawer_mediator {
  QGraphicsScene *scene;

public:
  explicit drawer_mediator(QGraphicsScene *scene);

  void draw_circle(const point_t &c, double r, const color_t &color);

  void draw_ellipse(const point_t &c, double ra, double rb,
                    const color_t &color);

  void draw_point(double x, double y, const color_t &color);

  void draw_points(std::vector<point_t> &points);

  void clear();
};

#endif // LAB_04_DRAWER_MEDIATOR_H
