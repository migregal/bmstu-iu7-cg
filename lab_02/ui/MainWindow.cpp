//
// Created by gregory on 18.02.2021.
//

#include "MainWindow.h"
#include "Instruments.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->movevemt_apply, &QPushButton::clicked, this,
            &MainWindow::movement_clicked);

    connect(ui->resizing_apply, &QPushButton::clicked, this,
            &MainWindow::resize_clicked);

    connect(ui->rotation_apply, &QPushButton::clicked, this,
            &MainWindow::rotate_clicked);

    connect(ui->step_forward_apply, &QPushButton::clicked, this,
            &MainWindow::forward_clicked);

    connect(ui->step_backward_apply, &QPushButton::clicked, this,
            &MainWindow::backward_clicked);

    connect(ui->reset_apply, &QPushButton::clicked, this,
            &MainWindow::reset_clicked);

    satellite = new SatelliteData::Satellite(ui->plot);

    set_coord_data();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::set_coord_data() {
    auto center = satellite->get_center();
    ui->center_coord_data->setText(
            QString("(%1; %2)")
                    .arg(QString::number(center.x(), 'g', 5))
                    .arg(QString::number(center.y(), 'g', 5))
    );
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (event->type() == QKeyEvent::KeyPress) {
        if (event->matches(QKeySequence::Undo)) {
            backward_clicked();
        } else if (event->matches(QKeySequence::Redo)) {
            forward_clicked();
        }
    }
}

void MainWindow::movement_clicked() {
    auto x = ui->movement_x->value();
    auto y = ui->movement_y->value();

    satellite->move(QPointF(x, y));
    set_coord_data();
}

void MainWindow::resize_clicked() {
    auto x = ui->resizing_x->value();
    auto y = ui->resizing_y->value();

    auto kx = ui->resizing_kx->value();
    auto ky = ui->resizing_ky->value();

    satellite->resize(QPointF(x, y), QPointF(kx, ky));
    set_coord_data();
}

void MainWindow::rotate_clicked() {
    auto x = ui->rotation_x->value();
    auto y = ui->rotation_y->value();
    auto alpha = ui->rotation_angle->value();

    satellite->rotate(
            QPointF(x, y),
            alpha,
            QPointF(x, y) != satellite->get_center()
    );
    set_coord_data();
}

void MainWindow::forward_clicked() {
    satellite->redo();
    ui->plot->replot();
    set_coord_data();
}

void MainWindow::backward_clicked() {
    satellite->undo();
    ui->plot->replot();
    set_coord_data();
}

void MainWindow::reset_clicked() {
    satellite->reset();
    ui->plot->replot();
    set_coord_data();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    ui->plot->yAxis->setRange(-15, 15);
    ui->plot->xAxis->setScaleRatio(ui->plot->yAxis);
    ui->plot->replot();
}
