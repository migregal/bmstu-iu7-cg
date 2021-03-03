//
// Created by gregory on 21.02.2021.
//

#include "SatelliteWings.h"
#include "Instruments.h"

using namespace SatelliteData;

SatelliteWings::SatelliteWings(QCustomPlot *plot) : plot(plot) {
    wing_1 = new QCPCurve(plot->xAxis, plot->yAxis);
    wing_1->setPen(QPen(Qt::black, 2));
    dw_1.append(QCPCurveData(0, 1., sqrt(0.75)));
    dw_1.append(QCPCurveData(1, 0.5, 2));
    dw_1.append(QCPCurveData(2, -2., 2));
    dw_1.append(QCPCurveData(3, -1.5, sqrt(0.75)));

    wing_1->data()->set(dw_1, true);

    wing_2 = new QCPCurve(plot->xAxis, plot->yAxis);
    wing_2->setPen(QPen(Qt::black, 2));

    for (auto p : dw_1) {
        dw_2.append(QCPCurveData(p.t, p.key, -p.value));
    }

    wing_2->data()->set(dw_2, true);
}

void SatelliteWings::move(const QPointF &m) {
    action();

    for (auto &i : dw_1)
        i = QCPCurveData(i.t, i.key + m.x(), i.value + m.y());
    wing_1->data()->set(dw_1, true);

    for (auto &i : dw_2)
        i = QCPCurveData(i.t, i.key + m.x(), i.value + m.y());
    wing_2->data()->set(dw_2, true);
}

void SatelliteWings::rotate(const QPointF &center, float rads) {
    action();

    for (auto &i : dw_1) {
        auto p = Instruments::rotate(center, QPointF(i.key, i.value), rads);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    wing_1->data()->set(dw_1, true);

    for (auto &i : dw_2) {
        auto p = Instruments::rotate(center, QPointF(i.key, i.value), rads);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    wing_2->data()->set(dw_2, true);
}

void SatelliteWings::resize(const QPointF &center, const QPointF &vect) {
    action();

    for (auto &i : dw_1) {
        auto p = Instruments::resize(center, QPointF(i.key, i.value), vect);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    wing_1->data()->set(dw_1, true);

    for (auto &i : dw_2) {
        auto p = Instruments::resize(center, QPointF(i.key, i.value), vect);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    wing_2->data()->set(dw_2, true);
}

void SatelliteWings::undo() {
    if (undo_stack.isEmpty())
        return;

    save_state(&redo_stack);
    load_state(&undo_stack);
}

void SatelliteWings::redo() {
    if (redo_stack.isEmpty())
        return;

    save_state(&undo_stack);
    load_state(&redo_stack);
}

inline void SatelliteWings::save_state(QStack<stacked_wings> *stack) {
    stack->push({dw_1, dw_2});
}

inline void SatelliteWings::load_state(QStack<stacked_wings> *stack) {
    if (stack->isEmpty())
        return;

    auto a = stack->pop();
    dw_1.clear();
    dw_1 += a.a;
    wing_1->data()->set(dw_1, true);

    dw_2.clear();
    dw_2 += a.b;
    wing_2->data()->set(dw_2, true);
}

void SatelliteWings::action() {
    save_state(&undo_stack);
    redo_stack.clear();
}
