//
// Created by gregory on 11.02.2021.
//

#ifndef LAB_01_CANVAS_H
#define LAB_01_CANVAS_H

#include <QWidget>

#include "../../utils/DataSet.h"

class Canvas : public QWidget {
private:
    QPointF min = QPointF(0, 0), max = QPointF(0, 0);
    QPointF min_r = QPointF(0, 0), max_r = QPointF(0, 0);

    DataSet datastore;

    void paintEvent(QPaintEvent *event) override;

    void drawCoordAxes(QPainter &qp, float k);

    void drawPoints(QPainter &qp, float k, float r);

    void drawInnerCircle(QPainter &qp, float k);

    void drawOuterCircle(QPainter &qp, float k);

    void drawTriangle(QPainter &qp, float k);

    inline float get_k();
    inline int get_x_coord(float x, float k);
    inline int get_y_coord(float y, float k);

public:
    explicit Canvas(QWidget* parent = nullptr) : QWidget(parent) {
        QPalette palette1;

        palette1.setColor(QPalette::Window, Qt::white);

        setAutoFillBackground(true);
        setPalette(palette1);
    };

    bool addPoint(int idx, float x, float y);

    bool buildTriangle();

    full_detailed get_triangle_info();

    void clearPoints();
};


#endif //LAB_01_CANVAS_H
