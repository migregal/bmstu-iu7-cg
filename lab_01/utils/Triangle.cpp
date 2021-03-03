//
// Created by gregory on 14.02.2021.
//

#include <cmath>
#include <utility>
#include "Triangle.h"

Triangle::Triangle(PointInfo a, PointInfo b, PointInfo c) : A(std::move(a)),
                                                            B(std::move(b)),
                                                            C(std::move(c)) {
    auto Ap = A.second, Bp = B.second, Cp = C.second;

    side_a = (float) sqrt(
            (Bp.x() - Cp.x()) * (Bp.x() - Cp.x()) +
            (Bp.y() - Cp.y()) * (Bp.y() - Cp.y())
    );
    side_b = (float) sqrt(
            (Cp.x() - Ap.x()) * (Cp.x() - Ap.x()) +
            (Cp.y() - Ap.y()) * (Cp.y() - Ap.y())
    );
    side_c = (float) sqrt(
            (Bp.x() - Ap.x()) * (Bp.x() - Ap.x()) +
            (Bp.y() - Ap.y()) * (Bp.y() - Ap.y())
    );

    square = 0.5f * (float) std::abs(
            Ap.x() * (Bp.y() - Cp.y()) +
            Bp.x() * (Cp.y() - Ap.y()) +
            Cp.x() * (Ap.y() - Bp.y())
    );
}

void Triangle::calculate_circles() {
    auto Ap = A.second, Bp = B.second, Cp = C.second;

    incircle.first = QPointF(
            (side_a * Ap.x() + side_b * Bp.x() + side_c * Cp.x()) /
            (side_a + side_b + side_c),
            (side_a * Ap.y() + side_b * Bp.y() + side_c * Cp.y()) /
            (side_a + side_b + side_c)
    );
    incircle.second = 2 * square / (side_a + side_b + side_c);

    auto d = 2 * (Ap.x() * (Bp.y() - Cp.y()) + Bp.x() * (Cp.y() - Ap.y()) +
                  Cp.x() * (Ap.y() - Bp.y()));

    outcircle.first = QPointF(
            ((Ap.x() * Ap.x() + Ap.y() * Ap.y()) * (Bp.y() - Cp.y()) +
             (Bp.x() * Bp.x() + Bp.y() * Bp.y()) * (Cp.y() - Ap.y()) +
             (Cp.x() * Cp.x() + Cp.y() * Cp.y()) * (Ap.y() - Bp.y())) / d,
            ((Ap.x() * Ap.x() + Ap.y() * Ap.y()) * (Cp.x() - Bp.x()) +
             (Bp.x() * Bp.x() + Bp.y() * Bp.y()) * (Ap.x() - Cp.x()) +
             (Cp.x() * Cp.x() + Cp.y() * Cp.y()) * (Bp.x() - Ap.x())) / d);
    outcircle.second = side_a * side_b * side_c / (4 * square);
}

float Triangle::get_square() const {
    return square;
}

std::tuple<float, float, float> Triangle::get_side_lens() {
    return std::make_tuple(side_a, side_b, side_c);
}

std::tuple<PointInfo, PointInfo, PointInfo> Triangle::get_coords() {
    return std::make_tuple(A, B, C);
}

std::pair<QPointF, float> Triangle::get_incircle() {
    return incircle;
}

std::pair<QPointF, float> Triangle::get_outcircle() {
    return outcircle;
}


