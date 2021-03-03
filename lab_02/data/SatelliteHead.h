//
// Created by gregory on 21.02.2021.
//

#ifndef LAB_02_SATELLITEHEAD_H
#define LAB_02_SATELLITEHEAD_H

#include "../ui/widgets/qcustomplot.h"
#include "RotatableEllipse.h"

namespace SatelliteData {
    struct stacked_head {
        QLineF a, b;
    };

    class SatelliteHead {
        QStack<stacked_head> undo_stack, redo_stack;

        RotatableEllipse *main_module;
        RotatableEllipse *window;
        QCPItemLine *antenna_1;
        QCPItemLine *antenna_2;

        inline void save_state(QStack<stacked_head> *stack);

        inline void load_state(QStack<stacked_head> *stack);

        inline void action();

    public:
        SatelliteHead(QCustomPlot *plot, QPointF center, float r);

        void move(const QPointF &movement);

        void rotate(const QPointF &center, float rads);

        void resize(const QPointF &center, QPointF vect);

        void undo();

        void redo();
    };
}

#endif //LAB_02_SATELLITEHEAD_H
