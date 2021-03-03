//
// Created by gregory on 14.02.2021.
//

#ifndef LAB_01_TRIANGLE_H
#define LAB_01_TRIANGLE_H

#include <tuple>

#include <QPointF>

typedef std::pair<int, QPointF> PointInfo;

class Triangle {
    PointInfo A, B, C;
    float side_a, side_b, side_c;
    float square;
    std::pair<QPointF, float> incircle, outcircle;
public:
    Triangle(PointInfo a, PointInfo b, PointInfo c);

    void calculate_circles();

    [[nodiscard]] float get_square() const;

    std::tuple<float, float, float> get_side_lens();

    std::tuple<PointInfo, PointInfo, PointInfo> get_coords();

    std::pair<QPointF, float> get_incircle();

    std::pair<QPointF, float> get_outcircle();
};


#endif //LAB_01_TRIANGLE_H
