//
// Created by gregory on 18.02.2021.
//

#include <QPainter>
#include "Satellite.h"
#include "Instruments.h"

using namespace SatelliteData;

Satellite::Satellite(QCustomPlot *plot) {
    head = new SatelliteHead(plot, QPointF(2.5, 0), 1);
    body = new SatelliteBody(plot, 1);
    wings = new SatelliteWings(plot);

    geom_center = QPointF(0, 0);
}

void Satellite::move(const QPointF &vect) {
    action();

    head->move(vect);
    body->move(vect);
    wings->move(vect);

    geom_center += vect;
}

void Satellite::rotate(QPointF center, float angle, bool rect_upd) {
    action();

    angle *= -M_PI / 180;

    head->rotate(center, angle);
    body->rotate(center, angle);
    wings->rotate(center, angle);

    if (!rect_upd)
        return;

    geom_center = Instruments::rotate(center, geom_center, angle);
}

void Satellite::resize(const QPointF &center, const QPointF &vect) {
    action();

    head->resize(center, vect);
    body->resize(center, vect);
    wings->resize(center, vect);

    geom_center = Instruments::resize(center, geom_center, vect);
}

void Satellite::reset() {
    while (!undo_stack.isEmpty()) {
        save_state(&redo_stack);
        load_state(&undo_stack);

        head->undo();
        body->undo();
        wings->undo();
    }
}

void Satellite::undo() {
    if (undo_stack.isEmpty())
        return;

    save_state(&redo_stack);
    load_state(&undo_stack);

    head->undo();
    body->undo();
    wings->undo();
}

void Satellite::redo() {
    if (redo_stack.isEmpty())
        return;

    save_state(&undo_stack);
    load_state(&redo_stack);

    head->redo();
    body->redo();
    wings->redo();
}

inline void Satellite::save_state(QStack<stacked_satellite> *stack) {
    stack->push({geom_center});
}

inline void Satellite::load_state(QStack<stacked_satellite> *stack) {
    if (stack->isEmpty())
        return;

    auto a = stack->pop();
    geom_center = a.center;
}

QPointF Satellite::get_center() {
    return geom_center;
}

void Satellite::action() {
    save_state(&undo_stack);
    redo_stack.clear();
}
