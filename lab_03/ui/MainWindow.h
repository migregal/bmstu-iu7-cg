//
// Created by gregory on 12.03.2021.
//

#ifndef LAB_03_MAINWINDOW_H
#define LAB_03_MAINWINDOW_H

#include <QBarSet>
#include <QChart>
#include <QLineSeries>
#include <QMainWindow>
#include <QSplineSeries>

#include <drawer_mediator.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

  Ui::MainWindow *ui;

  drawer_mediator *mediator;

  QMainWindow *chartWindow{};

  double measure_avg(int method, bool display, double r, int32_t step);

  double compare_methods(QtCharts::QBarSet *set, double r, int32_t step,
                         bool display);

  int32_t compare_method_steps(int alg, QtCharts::QLineSeries *series, double r,
                               int step);

protected:
  void on_method_selected();

  void on_color_selected();

  void on_draw_line_clicked();

  void on_draw_bunch_clicked();

  void on_compare_clicked();

  void on_compare_steps_clicked();

  void clear_screen();

  void paintEvent(QPaintEvent *event) override;

public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow() override;
};

#endif // LAB_03_MAINWINDOW_H
