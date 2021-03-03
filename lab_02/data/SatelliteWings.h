//
// Created by gregory on 21.02.2021.
//

#ifndef LAB_02_SATELLITEWINGS_H
#define LAB_02_SATELLITEWINGS_H

#include "../ui/widgets/qcustomplot.h"

namespace SatelliteData {
    struct stacked_wings {
        QVector<QCPCurveData> a, b;
    };

    class SatelliteWings {
        QCustomPlot *plot;

        QStack<stacked_wings> undo_stack, redo_stack;

        QVector<QCPCurveData> dw_1, dw_2;

        QCPCurve *wing_1;
        QCPCurve *wing_2;

        inline void save_state(QStack<stacked_wings> *stack);

        inline void load_state(QStack<stacked_wings> *stack);

        inline void action();

    public:
        explicit SatelliteWings(QCustomPlot *plot);

        void move(const QPointF &movement);

        void rotate(const QPointF &center, float rads);

        void resize(const QPointF &center, const QPointF &vect);

        void undo();

        void redo();
    };
}

#endif //LAB_02_SATELLITEWINGS_H
