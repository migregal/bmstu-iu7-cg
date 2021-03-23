//
// Created by gregory on 03.03.2021.
//

#ifndef LAB_04_DRAWER_MEDIATOR_H
#define LAB_04_DRAWER_MEDIATOR_H

#include <QGraphicsScene>
#include <datatypes.h>

class drawer_mediator {
  QGraphicsScene *scene;

public:
  explicit drawer_mediator(QGraphicsScene *scene);

  void draw_line(const point_t &p1, const point_t &p2, color_t color);

  void draw_point(double x, double y, const color_t &color);

  void clear();
};

#endif // LAB_04_DRAWER_MEDIATOR_H
