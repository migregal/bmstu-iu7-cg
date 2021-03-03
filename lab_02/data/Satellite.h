//
// Created by gregory on 18.02.2021.
//

#ifndef LAB_02_SATELLITE_H
#define LAB_02_SATELLITE_H

#include "RotatableEllipse.h"
#include "SatelliteBody.h"
#include "SatelliteWings.h"
#include "SatelliteHead.h"

namespace SatelliteData {
    struct stacked_satellite {
        QPointF center;
    };

    class Satellite {
        QStack<stacked_satellite> undo_stack, redo_stack;

        QPointF geom_center;

        SatelliteHead *head;
        SatelliteBody *body;
        SatelliteWings *wings;

        inline void save_state(QStack<stacked_satellite> *stack);

        inline void load_state(QStack<stacked_satellite> *stack);

        inline void action();

    public:
        explicit Satellite(QCustomPlot *plot);

        void rotate(QPointF center, float angle, bool rect_upd);

        void move(const QPointF &vect);

        void resize(const QPointF &center, const QPointF &vect);

        void reset();

        void undo();

        void redo();

        QPointF get_center();
    };
}

#endif //LAB_02_SATELLITE_H
