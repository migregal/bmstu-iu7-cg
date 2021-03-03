//
// Created by gregory on 21.02.2021.
//

#ifndef LAB_02_SATELLITEBODY_H
#define LAB_02_SATELLITEBODY_H


#include "../ui/widgets/qcustomplot.h"

namespace SatelliteData {
    struct stacked_body {
        QLineF a, b;
        QVector<QCPCurveData> arc;
    };

    class SatelliteBody {
        QCustomPlot *plot;

        QStack<stacked_body> undo_stack, redo_stack;

        QCPItemLine *line_1;
        QCPItemLine *line_2;

        QVector<QCPCurveData> arc;
        QCPCurve *line_3;

        inline void save_state(QStack<stacked_body> *stack);

        inline void load_state(QStack<stacked_body> *stack);

        inline void action();

    public:
        explicit SatelliteBody(QCustomPlot *plot, float r);

        void move(const QPointF &movement);

        void rotate(const QPointF &center, float rads);

        void resize(const QPointF &center, const QPointF &vect);

        void undo();

        void redo();
    };
}

#endif //LAB_02_SATELLITEBODY_H
