//
// Created by gregory on 11.02.2021.
//

#ifndef LAB_01_DATASET_H
#define LAB_01_DATASET_H

#include <tuple>
#include <QPointF>
#include <QList>
#include <QListIterator>

#include "Triangle.h"

typedef std::tuple<PointInfo, PointInfo, PointInfo> tr_detailed;
typedef std::pair<tr_detailed, std::pair<float, float>> full_detailed;

class DataSet {
    QList<PointInfo> points;
    Triangle *triangle = nullptr;

    inline void check_possible_triangle(PointInfo &a, PointInfo &b, PointInfo &c,
                                        float &max_r_diff);

public:
    QListIterator<std::pair<int, QPointF>> getPoints();

    bool addPoint(int idx, QPointF point);

    std::tuple<PointInfo, PointInfo, PointInfo> getTriangle();

    full_detailed getTriangleDetailed();

    bool findTriangle();

    std::pair<QPointF, float> getTriangleInnerCircle();

    std::pair<QPointF, float> getTriangleOuterCircle();

    void clearData();
};

#endif //LAB_01_DATASET_H
