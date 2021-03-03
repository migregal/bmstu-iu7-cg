//
// Created by gregory on 20.02.2021.
//

#ifndef LAB_02_ROTATABLEELLIPSE_H
#define LAB_02_ROTATABLEELLIPSE_H

#include "../ui/widgets/qcustomplot.h"

class RotatableEllipse {
    struct stacked_ellipse {
        QPointF c;
        QPointF r;
        QVector<QCPCurveData> points;
    };

    QStack<stacked_ellipse> undo_stack, redo_stack;

    QPointF c;
    QPointF r;

    QVector<QCPCurveData> points;

    QCPCurve curve;

    float inline get_x(float alpha, float t);

    float inline get_y(float alpha, float t);

    void inline calculate_arc(float t);

    inline void save_state(QStack<stacked_ellipse> *stack);

    inline void load_state(QStack<stacked_ellipse> *stack);

    inline void action();

public:
    explicit RotatableEllipse(
            const QCustomPlot *plot, const QPointF &center,
            QPointF r, const float t);

    void resize(const QPointF &center, const QPointF &vect);

    void movement(const QPointF &vect);

    void rotate(const QPointF &center, float rads);

    void undo();

    void redo();
};


#endif //LAB_02_ROTATABLEELLIPSE_H
