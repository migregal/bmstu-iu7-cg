//
// Created by gregory on 12.03.2021.
//

#ifndef LAB_04_MAINWINDOW_H
#define LAB_04_MAINWINDOW_H

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

using line_vect = std::vector<QtCharts::QLineSeries *>;

class MainWindow : public QMainWindow {
  Q_OBJECT

  Ui::MainWindow *ui;

  drawer_mediator *mediator;

  QMainWindow *chartWindow{};

  void check_ui();

  double compare_circle_methods(line_vect &series, int32_t r, int32_t step,
                                int32_t count, bool display = false);

  double compare_ellipse_methods(QtCharts::QBarSet *set, int32_t ra, int32_t rb,
                                 int32_t step, int32_t count,
                                 bool display = false);

protected:
  void on_figure_selected(int idx);

  void on_method_selected(int idx);

  void on_color_selected(int idx);

  void on_draw_circle_clicked();

  void on_draw_bunch_clicked();

  void on_compare_clicked();

  void clear_screen();

  void paintEvent(QPaintEvent *event) override;

public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow() override;
};

#endif // LAB_04_MAINWINDOW_H
