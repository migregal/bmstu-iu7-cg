//
// Created by gregory on 11.02.2021.
//

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "DataSet.h"

bool DataSet::addPoint(int idx, QPointF point) {
    auto p = PointInfo(idx, point);
    if (points.contains(p))
        return false;

    points.append(p);
    return true;
}

inline bool get_radiuses(Triangle *tr, float &R, float &r) {
    float S = tr->get_square();

    if (0 == S) return false;

    float side_a, side_b, side_c;
    std::tie(side_a, side_b, side_c) = tr->get_side_lens();

    R = side_a * side_b * side_c / (4 * S);
    r = 2 * S / (side_a + side_b + side_c);

    return true;
}

inline void DataSet::check_possible_triangle(PointInfo &a, PointInfo &b, PointInfo &c,
                                             float &max_r_diff) {
    auto tr = new Triangle(a, b, c);

    float r, R;

    if (!get_radiuses(tr, R, r))
        return;

    if (R - r <= max_r_diff) return;

    max_r_diff = R - r;
    delete triangle;
    triangle = tr;
}

bool DataSet::findTriangle() {
    float max = -1.0;

    for (auto a = points.begin(); a != points.end(); ++a)
        for (auto b = std::next(a); b != points.end(); ++b)
            for (auto c = std::next(b); c != points.end(); ++c)
                check_possible_triangle(*a, *b, *c, max);

    if (max < 0)
        return false;

    triangle->calculate_circles();
    return true;
}

QListIterator<PointInfo> DataSet::getPoints() {
    return QListIterator(points);
}

std::tuple<PointInfo, PointInfo, PointInfo> DataSet::getTriangle() {
    return triangle->get_coords();
}

full_detailed DataSet::getTriangleDetailed() {
    float R, r;

    get_radiuses(triangle, R, r);

    return full_detailed(
            triangle->get_coords(),
            {M_PI * r * r, M_PI * R * R}
    );
}

std::pair<QPointF, float> DataSet::getTriangleOuterCircle() {
    return triangle->get_outcircle();
}

std::pair<QPointF, float> DataSet::getTriangleInnerCircle() {
    return triangle->get_incircle();
}

void DataSet::clearData() {
    delete triangle;
    triangle = nullptr;

    points.clear();
}

