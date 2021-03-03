//
// Created by gregory on 20.02.2021.
//

#include <QPointF>

#include "RotatableEllipse.h"
#include "Instruments.h"

float inline RotatableEllipse::get_x(float alpha, float t) {
    return c.x() + r.x() * cos(alpha) * cos(t) - r.y() * sin(alpha) * sin(t);
}

float inline RotatableEllipse::get_y(float alpha, float t) {
    return c.y() + r.x() * cos(alpha) * sin(t) + r.y() * sin(alpha) * cos(t);
}

void inline RotatableEllipse::calculate_arc(float t=0) {
    points.clear();

    auto k = M_PI / 32;
    for (auto i = 0; i <= 64; ++i)
        points.append(QCPCurveData(
                i,
                get_x((float) (i * k), t),
                get_y((float) (i * k), t)
        ));

    curve.data()->set(points, true);
}

RotatableEllipse::RotatableEllipse(
        const QCustomPlot *const plot,
        const QPointF &center, const QPointF r,
        const float t = 0) :
        curve(plot->xAxis, plot->yAxis), c(center), r(r) {
    curve.setPen(QPen(Qt::black, 2));

    calculate_arc(t);
}

void RotatableEllipse::movement(const QPointF &vect) {
    action();

    c += vect;

    for (auto &i : points) {
        auto p = QPointF(i.key, i.value) + vect;
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    curve.data()->set(points, true);
}

void RotatableEllipse::resize(const QPointF &center, const QPointF &vect) {
    action();

    r = Instruments::resize(QPointF(0, 0), r, vect);
    c = Instruments::resize(center, c, vect);

    for (auto &i : points) {
        auto p = Instruments::resize(center, QPointF(i.key, i.value), vect);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    curve.data()->set(points, true);
}

void RotatableEllipse::rotate(const QPointF &center, const float rads) {
    action();

    c = Instruments::rotate(center, c, rads);

    for (auto &i : points) {
        auto p = Instruments::rotate(center, QPointF(i.key, i.value), rads);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    curve.data()->set(points, true);
}

void RotatableEllipse::undo() {
    if (undo_stack.isEmpty())
        return;

    save_state(&redo_stack);
    load_state(&undo_stack);
}

void RotatableEllipse::redo() {
    if (redo_stack.isEmpty())
        return;

    save_state(&undo_stack);
    load_state(&redo_stack);
}

inline void RotatableEllipse::save_state(QStack<stacked_ellipse> *stack) {
    stack->push({c, r, points});
}

inline void RotatableEllipse::load_state(QStack<stacked_ellipse> *stack) {
    if (stack->isEmpty())
        return;

    auto a = stack->pop();

    c = a.c;
    r = a.r;

    points.clear();
    points += a.points;
    curve.data()->set(points, true);
}

void RotatableEllipse::action() {
    save_state(&undo_stack);
    redo_stack.clear();
}
