//
// Created by gregory on 18.02.2021.
//

#ifndef LAB_02_MAINWINDOW_H
#define LAB_02_MAINWINDOW_H

#include "UiMainWindow.h"
#include "../data/Satellite.h"

class MainWindow : public QMainWindow {
    Ui::MainWindow *ui;

    SatelliteData::Satellite *satellite;

    inline void set_coord_data();

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
protected:

    void paintEvent(QPaintEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void movement_clicked();

    void resize_clicked();

    void rotate_clicked();

    void forward_clicked();

    void backward_clicked();

    void reset_clicked() ;
};


#endif //LAB_02_MAINWINDOW_H
