//
// Created by gregory on 21.02.2021.
//

#include "SatelliteBody.h"
#include "Instruments.h"

using namespace SatelliteData;

inline float distance(QPointF &p1, QPointF &p2) {
    return std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
}

inline QPointF get_arc_center(QPointF &p1, QPointF &p2, float r,
                              QPointF &closest) {
    QPointF middle{(p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2};
    QPointF dist{0.5 * (p1.x() - p2.x()), 0.5 * (p1.y() - p2.y())};

    // Длины полудиагоналей ромба
    float k = std::hypot(dist.x(), dist.y());
    k = sqrt(r * r - k * k) / k;

    QPointF pa{middle.x() + k * dist.y(), middle.y() - k * dist.x()},
            pb{middle.x() - k * dist.y(), middle.y() + k * dist.x()};

    if (distance(closest, pa) < distance(closest, pb))
        return pa;
    else
        return pb;
}

SatelliteBody::SatelliteBody(QCustomPlot *plot, float r) : plot(plot) {
    QPointF a{2, sqrt(0.75)},
            b{-3, sqrt(0.75)},
            c{2, -sqrt(0.75)},
            d{-3, -sqrt(0.75)};

    line_1 = new QCPItemLine(plot);
    line_1->setPen(QPen(Qt::black, 2));
    line_1->start->setCoords(a);
    line_1->end->setCoords(b);
    line_2 = new QCPItemLine(plot);
    line_2->setPen(QPen(Qt::black, 2));
    line_2->start->setCoords(c);
    line_2->end->setCoords(d);

    line_3 = new QCPCurve(plot->xAxis, plot->yAxis);
    line_3->setPen(QPen(Qt::black, 2));
    auto m = QPointF((a.x() + c.x()) / 2, (a.y() + c.y()) / 2);
    auto center = get_arc_center(b, d, r, m);

    float alpha = M_PI - atan2(center.y() - b.y(), center.x() - b.x());
    float beta = M_PI - atan2(center.y() - d.y(), center.x() - d.x());

    float step = (std::max(alpha, beta) - std::min(alpha, beta)) / 32;
    float min = std::min(alpha, beta);
    for (auto i = 0; i <= 32; ++i) {
        arc.append(
                QCPCurveData(i,
                             center.x() + r * cos(min + i * step),
                             center.y() + r * sin(min + i * step)
                )
        );
    }

    line_3->data()->set(arc, true);
}

void SatelliteBody::move(const QPointF &m) {
    action();

    line_1->start->setCoords(line_1->start->coords() + m);
    line_1->end->setCoords(line_1->end->coords() + m);

    line_2->start->setCoords(line_2->start->coords() + m);
    line_2->end->setCoords(line_2->end->coords() + m);

    for (auto &i : arc)
        i = QCPCurveData(i.t, i.key + m.x(), i.value + m.y());
    line_3->data()->set(arc, true);
}

void SatelliteBody::rotate(const QPointF &center, const float rads) {
    action();

    line_1->start->setCoords(
            Instruments::rotate(center, line_1->start->coords(), rads));
    line_1->end->setCoords(
            Instruments::rotate(center, line_1->end->coords(), rads));

    line_2->start->setCoords(
            Instruments::rotate(center, line_2->start->coords(), rads));
    line_2->end->setCoords(
            Instruments::rotate(center, line_2->end->coords(), rads));

    for (auto &i : arc) {
        auto p = Instruments::rotate(center, QPointF(i.key, i.value), rads);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    line_3->data()->set(arc, true);
}

void SatelliteBody::resize(const QPointF &center, const QPointF &vect) {
    action();

    line_1->start->setCoords(
            Instruments::resize(center, line_1->start->coords(), vect));
    line_1->end->setCoords(
            Instruments::resize(center, line_1->end->coords(), vect));

    line_2->start->setCoords(
            Instruments::resize(center, line_2->start->coords(), vect));
    line_2->end->setCoords(
            Instruments::resize(center, line_2->end->coords(), vect));

    for (auto &i : arc) {
        auto p = Instruments::resize(center, QPointF(i.key, i.value), vect);
        i = QCPCurveData(i.t, p.x(), p.y());
    }
    line_3->data()->set(arc, true);
}

void SatelliteBody::undo() {
    if (undo_stack.isEmpty())
        return;

    save_state(&redo_stack);
    load_state(&undo_stack);
}

void SatelliteBody::redo() {
    if (redo_stack.isEmpty())
        return;

    save_state(&undo_stack);
    load_state(&redo_stack);
}

inline void SatelliteBody::save_state(QStack<stacked_body> *stack) {
    stack->push(
            {
                    QLineF(line_1->start->key(), line_1->start->value(),
                           line_1->end->key(), line_1->end->value()),
                    QLineF(line_2->start->key(), line_2->start->value(),
                           line_2->end->key(), line_2->end->value()),
                    arc
            }
    );
}

inline void SatelliteBody::load_state(QStack<stacked_body> *stack) {
    if (stack->isEmpty())
        return;

    auto a = stack->pop();

    line_1->start->setCoords(a.a.p1());
    line_1->end->setCoords(a.a.p2());
    line_2->start->setCoords(a.b.p1());
    line_2->end->setCoords(a.b.p2());

    arc.clear();
    arc += a.arc;
    line_3->data()->set(arc, true);
}

void SatelliteBody::action() {
    save_state(&undo_stack);
    redo_stack.clear();
}
