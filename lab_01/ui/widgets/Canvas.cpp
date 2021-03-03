//
// Created by gregory on 11.02.2021.
//

#include <cmath>
#include <QPainter>
#include <QMessageBox>
#include <QFont>

#include "Canvas.h"

inline float Canvas::get_k() {
    auto t = datastore.getTriangleOuterCircle();

    max_r = QPointF(
            std::max(max.x(), t.first.x() + t.second),
            std::max(max.y(), t.first.y() + t.second)
    );
    min_r = QPointF(
            std::min(min.x(), t.first.x() - t.second),
            std::min(min.y(), t.first.y() - t.second)
    );

    float kx = 0.85f * (float) width() / (float) (max_r.x() - min_r.x());
    float ky = 0.85f * (float) height() / (float) (max_r.y() - min_r.y());

    float k = std::min(kx, ky);

    return k;
}


void Canvas::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    if (!datastore.getPoints().hasNext())
        return;

    QPainter qp;
    qp.begin(this);
    qp.setRenderHint(QPainter::Antialiasing);

    auto k = get_k();

    drawCoordAxes(qp, k);
    drawTriangle(qp, k);
    drawInnerCircle(qp, k);
    drawOuterCircle(qp, k);
    drawPoints(qp, k, 5);

    qp.end();
}

inline int Canvas::get_x_coord(float x, float k) {
    return (int) (0.075f * (float) width() + (x - min_r.x()) * k);
}

inline int Canvas::get_y_coord(float y, float k) {
    return (int) (0.925f * (float) height() - (y - min_r.y()) * k);
}

void Canvas::drawCoordAxes(QPainter &qp, float k) {
    auto br = qp.brush();
    auto pen = qp.pen();

    qp.setPen(QPen());

    qp.drawLine((int) (0.025 * width()), get_y_coord(0, k),
                (int) (0.975 * width()), get_y_coord(0, k));
    qp.drawLine(get_x_coord(0, k), (int) (0.025 * height()),
                get_x_coord(0, k), (int) (0.975 * height()));

    qp.setPen(pen);
    qp.setBrush(br);
}

void Canvas::drawTriangle(QPainter &qp, float k) {
    auto br = qp.brush();
    auto pen = qp.pen();

    qp.setBrush(QBrush());
    qp.setPen(QPen(Qt::red, 2));

    PointInfo a, b, c;
    std::tie(a, b, c) = datastore.getTriangle();

    qp.drawLine(get_x_coord(a.second.x(), k), get_y_coord(a.second.y(), k),
                get_x_coord(b.second.x(), k), get_y_coord(b.second.y(), k));
    qp.drawLine(get_x_coord(a.second.x(), k), get_y_coord(a.second.y(), k),
                get_x_coord(c.second.x(), k), get_y_coord(c.second.y(), k));
    qp.drawLine(get_x_coord(b.second.x(), k), get_y_coord(b.second.y(), k),
                get_x_coord(c.second.x(), k), get_y_coord(c.second.y(), k));

    qp.setPen(pen);
    qp.setBrush(br);
}

void Canvas::drawOuterCircle(QPainter &qp, float k) {
    auto br = qp.brush();
    auto pen = qp.pen();

    qp.setPen(QPen(Qt::darkBlue, 2));
    auto t = datastore.getTriangleOuterCircle();

    qp.drawEllipse(
            QPoint(get_x_coord(t.first.x(), k), get_y_coord(t.first.y(), k)),
            (int) (t.second * k), (int) (t.second * k)
    );

    qp.setPen(pen);
    qp.setBrush(br);
}

void Canvas::drawInnerCircle(QPainter &qp, float k) {
    auto br = qp.brush();
    auto pen = qp.pen();

    qp.setPen(QPen(Qt::darkMagenta, 2));
    auto t = datastore.getTriangleInnerCircle();

    qp.drawEllipse(
            QPoint(get_x_coord(t.first.x(), k), get_y_coord(t.first.y(), k)),
            (int) (t.second * k), (int) (t.second * k)
    );

    qp.setPen(pen);
    qp.setBrush(br);
}

inline QPointF get_normalized_r(QPoint center, QPoint point) {
    auto v = QPoint(point.x() - center.x(), point.y() - center.y());

    v /= sqrt(v.x() * v.x() + v.y() * v.y());

    return v;
}

void Canvas::drawPoints(QPainter &qp, float k, float r) {
    auto br = qp.brush();
    auto pen = qp.pen();
    auto font = qp.font();

    qp.setBrush(QBrush(Qt::SolidPattern));
    qp.setPen(QPen());
    qp.setFont(QFont("Roboto", 10, QFont::Bold));

    auto c = QPoint(
            get_x_coord((float) (max_r.x() + min_r.x()) / 2, k),
            get_y_coord((float) (max_r.y() + min_r.y()) / 2, k)
    );

    for (auto iter = datastore.getPoints(); iter.hasNext();) {
        auto pi = iter.next();
        auto p = QPoint(get_x_coord(pi.second.x(), k),
                        get_y_coord(pi.second.y(), k));

        qp.drawEllipse(p, (int) r, (int) r);
        auto m = pi.first > 9 ? 7 : 3.5;
        auto v = get_normalized_r(c, p) * m * r;


        qp.drawText(p.x() + v.x() - m * r / 4, p.y() + v.y() - 1.5 * r,
                    m / 2 * r, 3 * r,
                    Qt::AlignCenter, QString("%1").arg(pi.first)
        );
    }

    qp.setFont(font);
    qp.setPen(pen);
    qp.setBrush(br);
}

bool Canvas::addPoint(int idx, float x, float y) {
    if (!datastore.addPoint(idx, QPointF(x, y)))
        return false;

    max = QPointF(std::max((float) max.x(), x), std::max((float) max.y(), y));
    min = QPointF(std::min((float) min.x(), x), std::min((float) min.y(), y));
    return true;
}

bool Canvas::buildTriangle() {
    return datastore.findTriangle();
}

full_detailed Canvas::get_triangle_info() {
    return datastore.getTriangleDetailed();
}

void Canvas::clearPoints() {
    min = QPointF(0, 0);
    max = QPointF(0, 0);

    datastore.clearData();
}
