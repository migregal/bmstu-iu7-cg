//
// Created by gregory on 12.03.2021.
//

#include <chrono>
#include <cmath>
#include <vector>

#include <QStringListModel>

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

#include <QGuiApplication>
#include <QScreen>
#include <QStyle>

#include <design.h>

#include <MainWindow.h>
#include <request.h>

extern const char *alg_titles[ALGS_TITLES_LEN];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  auto *scene = new QGraphicsScene(this);
  ui->canvas->setScene(scene);
  ui->canvas->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  ui->canvas->setRenderHint(QPainter::Antialiasing);

  mediator = new drawer_mediator(scene);

  auto model = new QStringListModel(this);

  QStringList list;

  for (auto &alg_title : alg_titles) {
    list << alg_title;
  }

  model->setStringList(list);

  ui->method_list->setModel(model);
  ui->method_list->setEditTriggers(QAbstractItemView::NoEditTriggers);

  connect(ui->method_list, &QListView::clicked, this,
          &MainWindow::on_method_selected);

  connect(ui->black_check, &QRadioButton::clicked, this,
          &MainWindow::on_color_selected);
  connect(ui->background_check, &QRadioButton::clicked, this,
          &MainWindow::on_color_selected);

  connect(ui->circle_check, &QRadioButton::clicked, this,
          &MainWindow::on_figure_selected);
  connect(ui->ellipse_check, &QRadioButton::clicked, this,
          &MainWindow::on_figure_selected);

  connect(ui->coords_apply, &QPushButton::clicked, this,
          &MainWindow::on_draw_circle_clicked);
  connect(ui->bunch_apply, &QPushButton::clicked, this,
          &MainWindow::on_draw_bunch_clicked);

  connect(ui->cmp_apply, &QPushButton::clicked, this,
          &MainWindow::on_compare_clicked);

  connect(ui->clear_screen_apply, &QPushButton::clicked, this,
          &MainWindow::clear_screen);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::check_ui() {
  if (!ui->black_check->isChecked() && !ui->background_check->isChecked())
    return;

  if (ui->method_list->selectionModel()->selectedRows().isEmpty())
    return;

  if (!ui->circle_check->isChecked() && !ui->ellipse_check->isChecked())
    return;

  ui->coords_apply->setEnabled(true);
  ui->bunch_apply->setEnabled(true);
}

void MainWindow::on_figure_selected() {
  check_ui();

  if (ui->circle_check->isChecked()) {
    ui->rb_spinbox->setEnabled(false);
    ui->bunch_r_b->setEnabled(false);
    return;
  }

  if (ui->ellipse_check->isChecked()) {
    ui->rb_spinbox->setEnabled(true);
    ui->bunch_r_b->setEnabled(true);
  }
}

void MainWindow::on_method_selected() { check_ui(); }

void MainWindow::on_color_selected() { check_ui(); }

void MainWindow::on_draw_circle_clicked() {
  auto method_n = ui->method_list->selectionModel()->selectedIndexes()[0].row();

  auto arg = args{.method = CANONICAL + method_n};

  if (ui->circle_check->isChecked()) {
    arg.command = DRAW_CIRCLE;
    arg.circle = {{ui->xc_spinbox->value(), ui->yc_spinbox->value()},
                  ui->ra_spinbox->value()};
  } else {
    arg.command = DRAW_ELLIPSE;
    arg.ellipse = {{ui->xc_spinbox->value(), ui->yc_spinbox->value()},
                   ui->ra_spinbox->value(),
                   ui->rb_spinbox->value()};
  }

  if (ui->black_check->isChecked())
    request(*mediator, arg).execute({0, 0, 0});
  else
    request(*mediator, arg).execute({255, 255, 255});
}

void MainWindow::on_draw_bunch_clicked() {
  auto method_n = ui->method_list->selectionModel()->selectedIndexes()[0].row();
  auto arg = args{.method = CANONICAL + method_n};

  if (ui->circle_check->isChecked()) {
    arg.command = DRAW_CIRCLE_BUNCH;
    arg.c_bunch =
        circle_bunch_t{.circle = {.center = {ui->canvas->width() / 2.0,
                                             ui->canvas->height() / 2.0},
                                  .r = ui->bunch_r_a->value()},
                       .count = ui->bunch_count->value(),
                       .step = (int32_t)ui->bunch_step->value()};
  } else {
    arg.command = DRAW_ELLIPSE_BUNCH;
    arg.e_bunch =
        ellipse_bunch_t{.ellipse = {.center = {ui->canvas->width() / 2.0,
                                               ui->canvas->height() / 2.0},
                                    .ra = ui->bunch_r_a->value(),
                                    .rb = ui->bunch_r_b->value()},
                        .count = ui->bunch_count->value(),
                        .step = ui->bunch_step->value()};
  }

  if (ui->black_check->isChecked())
    request(*mediator, arg).execute({0, 0, 0});
  else
    request(*mediator, arg).execute({255, 255, 255});
}

double MainWindow::compare_circle_methods(QtCharts::QBarSet *set, int32_t r,
                                          int32_t step, int32_t count,
                                          bool display) {
  auto max = 0.;
  auto d = std::vector<double>();

  circle_bunch_t bunch{.circle{{}, r}, .count = count, .step = step};
  auto arg = args{.command = MEASURE_CIRCLE_TIMES,
                  .m_c_bunch = {.bunch = bunch, .time = &d}};

  request{*mediator, arg}.execute({}, display);

  for (auto &i : d) {
    *set << i;
    max = std::max(max, i);
  }

  return max;
}

double MainWindow::compare_ellipse_methods(QtCharts::QBarSet *set, int32_t ra,
                                           int32_t rb, int32_t step,
                                           int32_t count, bool display) {
  auto max = 0.;
  auto d = std::vector<double>();

  ellipse_bunch_t bunch{.ellipse{{}, ra, rb}, .count = count, .step = step};
  auto arg = args{.command = MEASURE_ELLIPSE_TIMES,
                  .m_e_bunch = {.bunch = bunch, .time = &d}};

  request{*mediator, arg}.execute({}, display);

  for (auto &i : d) {
    *set << i;
    max = std::max(max, i);
  }

  return max;
}

void MainWindow::on_compare_clicked() {
  //    auto r = ui->bunch_r->value();
  //    auto step = ui->bunch_step->value();
  auto r = 15;
  auto step = 10;
  auto count = 10;

  auto *chart = new QtCharts::QChart();
  chart->setTitle(QString("Сравнение времени работы алгоритмов<br>Радиус "
                          "окружности: %1 Шаг: %2 Кол-во: %3")
                      .arg(r)
                      .arg(step)
                      .arg(count));
  chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

  auto *series = new QtCharts::QBarSeries();
  chart->addSeries(series);

  auto *set0 = new QtCharts::QBarSet("Время окружность");
  auto max = compare_circle_methods(set0, r, step, count);
  series->append(set0);

  auto *set1 = new QtCharts::QBarSet("Время эллипс");
  max = std::max(max, compare_ellipse_methods(set1, r, 2 * r, step, count));
  series->append(set1);

  QStringList categories;
  for (auto &alg_title : alg_titles) {
    categories << alg_title;
  }

  auto *axisX = new QtCharts::QBarCategoryAxis();
  axisX->append(categories);
  axisX->setTitleText("Метод");
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  auto *axisY = new QtCharts::QValueAxis();
  axisY->setRange(0, 1.2 * max);
  axisY->setTitleText("Время, мкс");
  axisY->applyNiceNumbers();
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);

  auto *chartView = new QtCharts::QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  chartWindow = new QMainWindow();
  chartWindow->setCentralWidget(chartView);
  chartWindow->resize(420, 300);
  chartWindow->setGeometry(QStyle::alignedRect(
      Qt::LeftToRight, Qt::AlignCenter, chartWindow->size(),
      QGuiApplication::primaryScreen()->availableGeometry()));
  chartWindow->show();
}

void MainWindow::clear_screen() {
  request(*mediator, {.command = CLEAR_SCREEN}).execute({});

  ui->method_list->selectionModel()->clear();

  ui->black_check->setAutoExclusive(false);
  ui->black_check->setChecked(false);
  ui->black_check->setAutoExclusive(true);

  ui->background_check->setAutoExclusive(false);
  ui->background_check->setChecked(false);
  ui->background_check->setAutoExclusive(true);

  ui->circle_check->setAutoExclusive(false);
  ui->circle_check->setChecked(false);
  ui->circle_check->setAutoExclusive(true);

  ui->ellipse_check->setAutoExclusive(false);
  ui->ellipse_check->setChecked(false);
  ui->ellipse_check->setAutoExclusive(true);

  ui->rb_spinbox->setEnabled(true);
  ui->bunch_r_b->setEnabled(true);

  ui->coords_apply->setEnabled(false);
  ui->bunch_apply->setEnabled(false);
}

void MainWindow::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  auto rcontent = ui->canvas->contentsRect();
  ui->canvas->scene()->setSceneRect(0, 0, rcontent.width(), rcontent.height());
}
