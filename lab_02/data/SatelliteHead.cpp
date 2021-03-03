//
// Created by gregory on 21.02.2021.
//

#include "Instruments.h"
#include "SatelliteHead.h"

using namespace SatelliteData;

SatelliteHead::SatelliteHead(QCustomPlot *plot, QPointF c, float r) {
    main_module = new RotatableEllipse(plot, c, QPointF(r, r), 0);
    window = new RotatableEllipse(plot, c, QPointF(r / 2, r / 2), 0);

    antenna_1 = new QCPItemLine(plot);
    antenna_1->setPen(QPen(Qt::black, 2));
    antenna_1->start->setCoords(c.x(), -r);
    antenna_1->end->setCoords(c.x() + r, -2 * r);

    antenna_2 = new QCPItemLine(plot);
    antenna_2->setPen(QPen(Qt::black, 2));
    antenna_2->start->setCoords(c.x(), r);
    antenna_2->end->setCoords(c.x() + r, 2 * r);
}

void SatelliteHead::move(const QPointF &movement) {
    action();

    main_module->movement(movement);
    window->movement(movement);

    antenna_1->start->setCoords(antenna_1->start->coords() + movement);
    antenna_1->end->setCoords(antenna_1->end->coords() + movement);

    antenna_2->start->setCoords(antenna_2->start->coords() + movement);
    antenna_2->end->setCoords(antenna_2->end->coords() + movement);
}

void SatelliteHead::rotate(const QPointF &c, float rads) {
    action();

    main_module->rotate(c, rads);
    window->rotate(c, rads);

    antenna_1->start->setCoords(
            Instruments::rotate(c, antenna_1->start->coords(), rads));
    antenna_1->end->setCoords(
            Instruments::rotate(c, antenna_1->end->coords(), rads));

    antenna_2->start->setCoords(
            Instruments::rotate(c, antenna_2->start->coords(), rads));
    antenna_2->end->setCoords(
            Instruments::rotate(c, antenna_2->end->coords(), rads));
}

void SatelliteHead::resize(const QPointF &c, QPointF vect) {
    action();

    main_module->resize(c, vect);
    window->resize(c, vect);

    antenna_1->start->setCoords(
            Instruments::resize(c, antenna_1->start->coords(), vect));
    antenna_1->end->setCoords(
            Instruments::resize(c, antenna_1->end->coords(), vect));

    antenna_2->start->setCoords(
            Instruments::resize(c, antenna_2->start->coords(), vect));
    antenna_2->end->setCoords(
            Instruments::resize(c, antenna_2->end->coords(), vect));
}

void SatelliteHead::undo() {
    main_module->undo();
    window->undo();

    if (undo_stack.isEmpty()) {
        return;
    }

    save_state(&redo_stack);
    load_state(&undo_stack);
}

void SatelliteHead::redo() {
    main_module->redo();
    window->redo();

    if (redo_stack.isEmpty())
        return;

    save_state(&undo_stack);
    load_state(&redo_stack);
}

inline void SatelliteHead::save_state(QStack<stacked_head> *stack) {
    stack->push(
            {
                    QLineF(antenna_1->start->key(), antenna_1->start->value(),
                           antenna_1->end->key(), antenna_1->end->value()),
                    QLineF(antenna_2->start->key(), antenna_2->start->value(),
                           antenna_2->end->key(), antenna_2->end->value())
            }
    );
}

inline void SatelliteHead::load_state(QStack<stacked_head> *stack) {
    if (stack->isEmpty())
        return;

    auto a = stack->pop();

    antenna_1->start->setCoords(a.a.p1());
    antenna_1->end->setCoords(a.a.p2());
    antenna_2->start->setCoords(a.b.p1());
    antenna_2->end->setCoords(a.b.p2());
}

void SatelliteHead::action() {
    save_state(&undo_stack);
    redo_stack.clear();
}
