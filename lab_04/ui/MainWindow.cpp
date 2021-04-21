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

  ui->method_list->addItem("");
  for (auto &alg_title : alg_titles)
    ui->method_list->addItem(alg_title);

  ui->figure_check->addItems({"", "Окружность", "Эллипс"});

  ui->color_check->addItems({"", "Черный", "Цвет фона"});

  ui->unused_list->addItems({"", "Rн", "Rк", "Шаг", "N"});

  connect(ui->method_list, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::on_method_selected);
  connect(ui->figure_check, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::on_figure_selected);
  connect(ui->color_check, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::on_color_selected);
  connect(ui->unused_list, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::on_unused_selected);

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
  if (ui->color_check->currentIndex() == 0) {
    ui->coords_apply->setEnabled(false);
    ui->bunch_apply->setEnabled(false);
    return;
  }

  if (ui->method_list->currentIndex() == 0) {
    ui->coords_apply->setEnabled(false);
    ui->bunch_apply->setEnabled(false);
    return;
  }

  if (ui->figure_check->currentIndex() == 0) {
    ui->coords_apply->setEnabled(false);
    ui->bunch_apply->setEnabled(false);
    return;
  }

  if (ui->unused_list->currentIndex() == 0) {
    ui->coords_apply->setEnabled(false);
    ui->bunch_apply->setEnabled(false);
    return;
  }

  ui->coords_apply->setEnabled(true);
  ui->bunch_apply->setEnabled(true);
}

void MainWindow::on_figure_selected(int idx) {
  check_ui();
  on_unused_selected(ui->unused_list->currentIndex());

  if (1 == idx) {
    ui->rb_spinbox->setEnabled(false);
    ui->bunch_r_b->setEnabled(false);
    return;
  }

  if (2 == idx || 0 == idx) {
    ui->rb_spinbox->setEnabled(true);
    ui->bunch_r_b->setEnabled(true);
    ui->bunch_r_b_k->setEnabled(true);
    return;
  }
}

void MainWindow::on_method_selected(int idx) { check_ui(); }

void MainWindow::on_color_selected(int idx) { check_ui(); }

void MainWindow::on_unused_selected(int idx) {
  check_ui();

  ui->bunch_r_a->setEnabled(true);
  ui->bunch_r_a_k->setEnabled(true);

  auto el = (ui->figure_check->currentIndex() == 2);
  ui->bunch_r_b->setEnabled(el);
  ui->bunch_r_b_k->setEnabled(el);

  ui->bunch_step_a->setEnabled(true);
  ui->bunch_step_b->setEnabled(true);
  ui->bunch_count->setEnabled(true);

  if (0 == idx)
    return;

  if (1 == idx) {
    ui->bunch_r_a->setEnabled(false);
    ui->bunch_r_b->setEnabled(false);
    return;
  }

  if (2 == idx) {
    ui->bunch_r_a_k->setEnabled(false);
    ui->bunch_r_b_k->setEnabled(false);
    return;
  }

  if (3 == idx) {
    ui->bunch_step_a->setEnabled(false);
    ui->bunch_step_b->setEnabled(false);
    return;
  }

  if (4 == idx) {
    ui->bunch_count->setEnabled(false);
    return;
  }
}

void MainWindow::on_draw_circle_clicked() {
  auto method_n = ui->method_list->currentIndex() - 1;

  auto arg = args{.method = CANONICAL + method_n};

  if (ui->figure_check->currentIndex() == 1) {
    arg.command = DRAW_CIRCLE;
    arg.circle = {{ui->xc_spinbox->value(), ui->yc_spinbox->value()},
                  ui->ra_spinbox->value()};
  }

  if (ui->figure_check->currentIndex() == 2) {
    arg.command = DRAW_ELLIPSE;
    arg.ellipse = {{ui->xc_spinbox->value(), ui->yc_spinbox->value()},
                   ui->ra_spinbox->value(),
                   ui->rb_spinbox->value()};
  }

  if (ui->color_check->currentIndex() == 1)
    request(*mediator, arg).execute({0, 0, 0});
  else if (ui->color_check->currentIndex() == 2)
    request(*mediator, arg).execute({255, 255, 255});
}

void MainWindow::on_draw_bunch_clicked() {
  auto method_n = ui->method_list->currentIndex() - 1;
  auto arg = args{.method = CANONICAL + method_n};

  if (ui->figure_check->currentIndex() == 1) {
    arg.command = DRAW_CIRCLE_BUNCH;
    arg.c_bunch = circle_bunch_t{
        .circle = {.center = {ui->bunch_xc->value(), ui->bunch_yc->value()},
                   .r = ui->bunch_r_a->value()},
        .count = ui->bunch_count->value(),
        .step = (int32_t)ui->bunch_step_a->value()};
  }

  if (ui->figure_check->currentIndex() == 2) {
    arg.command = DRAW_ELLIPSE_BUNCH;
    arg.e_bunch = ellipse_bunch_t{
        .ellipse = {.center = {ui->bunch_xc->value(), ui->bunch_yc->value()},
                    .ra = ui->bunch_r_a->value(),
                    .rb = ui->bunch_r_b->value()},
        .count = ui->bunch_count->value(),
        .step = ui->bunch_step_a->value()};
  }

  if (ui->color_check->currentIndex() == 1)
    request(*mediator, arg).execute({0, 0, 0});
  else if (ui->color_check->currentIndex() == 2)
    request(*mediator, arg).execute({255, 255, 255});
}

double MainWindow::compare_circle_methods(line_vect &series, int32_t r,
                                          int32_t step, int32_t count,
                                          bool display) {
  auto max = 0.;
  auto d = std::vector<timed_vect>();

  circle_bunch_t bunch{.circle{{}, r}, .count = count, .step = step};
  auto arg = args{.command = MEASURE_CIRCLE_TIMES,
                  .m_c_bunch = {.bunch = bunch, .time = &d}};

  request{*mediator, arg}.execute({}, display);

  for (auto &i : d) {
    auto ser = new QtCharts::QLineSeries();
    for (auto &j : i) {
      ser->append(j.first, j.second);
      max = std::max(max, j.second);
    }
    series.push_back(ser);
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
  auto r = 0;
  auto step = 10;
  auto count = 10;

  auto *chart = new QtCharts::QChart();
  chart->setTitle(QString("Сравнение времени работы алгоритмов<br> Шаг: %1 "
                          "Кол-во: %2")
                      .arg(step)
                      .arg(count));
  chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

  auto *axisX = new QtCharts::QValueAxis();
  axisX->setTitleText("Радиус окружности");
  chart->addAxis(axisX, Qt::AlignBottom);

  auto *axisY = new QtCharts::QValueAxis();
  axisY->setTitleText("Время, мкс");
  axisY->applyNiceNumbers();
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);

  auto *chartView = new QtCharts::QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  auto v = line_vect();
  auto max = compare_circle_methods(v, r, step, count);

  for (auto s = 0; s < v.size(); ++s) {
    chart->addSeries(v.at(s));
    v.at(s)->setName(alg_titles[s]);
    v.at(s)->attachAxis(axisX);
    v.at(s)->attachAxis(axisY);
  }
  axisY->setRange(0, 1.1 * max);

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

  ui->figure_check->setCurrentIndex(0);
  ui->color_check->setCurrentIndex(0);
  ui->method_list->setCurrentIndex(0);
  ui->unused_list->setCurrentIndex(0);

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
