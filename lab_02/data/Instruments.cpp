//
// Created by gregory on 22.02.2021.
//

#include <cmath>
#include "Instruments.h"


QPointF Instruments::rotate(const QPointF &center, QPointF p, float rads) {
    float c = cos(rads), s = sin(rads);
    float d_x = p.x() - center.x(), d_y = p.y() - center.y();

    return QPointF(
            center.x() + d_x * c + d_y * s,
            center.y() - d_x * s + d_y * c
    );
}

QPointF Instruments::resize(const QPointF &center, QPointF point, QPointF k) {
    return QPointF(
            k.x() * point.x() + center.x() * (1 - k.x()),
            k.y() * point.y() + center.y() * (1 - k.y())
    );
}
