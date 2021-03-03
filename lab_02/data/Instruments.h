//
// Created by gregory on 22.02.2021.
//

#ifndef LAB_02_INSTRUMENTS_H
#define LAB_02_INSTRUMENTS_H

#include <QPointF>

class Instruments {
public:
    static QPointF rotate(const QPointF &center, QPointF point, float rads);

    static QPointF resize(const QPointF &center, QPointF point, QPointF k);
};


#endif //LAB_02_INSTRUMENTS_H
