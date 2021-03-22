//
// Created by gregory on 12.03.2021.
//

#include <chrono>

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
#include <cmath>
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

  connect(ui->coords_apply, &QPushButton::clicked, this,
          &MainWindow::on_draw_line_clicked);
  connect(ui->bunch_apply, &QPushButton::clicked, this,
          &MainWindow::on_draw_bunch_clicked);

  connect(ui->cmp_apply, &QPushButton::clicked, this,
          &MainWindow::on_compare_clicked);

  connect(ui->cmp_steps_apply, &QPushButton::clicked, this,
          &MainWindow::on_compare_steps_clicked);

  connect(ui->clear_screen_apply, &QPushButton::clicked, this,
          &MainWindow::clear_screen);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_method_selected() {
  if (!ui->black_check->isChecked() && !ui->background_check->isChecked())
    return;

  ui->coords_apply->setEnabled(true);
  ui->bunch_apply->setEnabled(true);
}

void MainWindow::on_color_selected() {
  if (ui->method_list->selectionModel()->selectedRows().isEmpty())
    return;

  ui->coords_apply->setEnabled(true);
  ui->bunch_apply->setEnabled(true);
}

void MainWindow::on_draw_line_clicked() {
  auto line = line_t{
      .a = {(double)ui->x1_spinbox->value(), (double)ui->y1_spinbox->value()},
      .b = {(double)ui->x2_spinbox->value(), (double)ui->y2_spinbox->value()}};

  auto method_n = ui->method_list->selectionModel()->selectedIndexes()[0].row();

  auto arg = args{.command = DRAW_LINE, .method = DDA + method_n, .line = line};

  if (ui->black_check->isChecked())
    request(*mediator, arg).execute({0, 0, 0});
  else
    request(*mediator, arg).execute({255, 255, 255});
}

void MainWindow::on_draw_bunch_clicked() {
  auto bunch =
      bunch_t{.center = {ui->canvas->width() / 2.0, ui->canvas->height() / 2.0},
              .r = (uint32_t)ui->bunch_r->value(),
              .step = (uint32_t)ui->bunch_step->value()};

  auto method_n = ui->method_list->selectionModel()->selectedIndexes()[0].row();

  auto arg =
      args{.command = DRAW_BUNCH, .method = DDA + method_n, .bunch = bunch};

  if (ui->black_check->isChecked())
    request(*mediator, arg).execute({0, 0, 0});
  else
    request(*mediator, arg).execute({255, 255, 255});
}

inline uint64_t clock(request &req, bool display) {
  auto start = std::chrono::steady_clock::now();
  req.execute({}, display);
  auto end = std::chrono::steady_clock::now();

  return std::chrono::duration_cast<std::chrono::microseconds>(end - start)
      .count();
}

double MainWindow::measure_avg(int method, bool display) {
  auto bunch =
      bunch_t{.center = {ui->canvas->width() / 2.0, ui->canvas->height() / 2.0},
              .r = (uint32_t)ui->bunch_r->value(),
              .step = (uint32_t)ui->bunch_step->value()};
  auto arg = args{.command = DRAW_BUNCH, .method = method, .bunch = bunch};
  auto req = request(*mediator, arg);

  auto res = 0.0;

  for (int i = 0; i < 100; ++i) {
    res += clock(req, display);
    request(*mediator, {.command = CLEAR_SCREEN}).execute({});
  }

  return res / 100;
}

double MainWindow::compare_methods(QtCharts::QBarSet *set,
                                   bool display = false) {
  double measure, max = 0;

  for (int fooInt = DDA; fooInt <= STD; fooInt++) {
    algs foo = static_cast<algs>(fooInt);
    measure = measure_avg(foo, display);
    max = std::max(max, measure);
    *set << measure;
  }

  return max;
}

inline double to_rads(double degree) { return degree * M_PI / 180; }

int32_t MainWindow::compare_method_steps(int alg,
                                         QtCharts::QLineSeries *series) {

  auto arg =
      args{.command = DRAW_LINE_STEP_COUNT, .method = static_cast<algs>(alg)};

  auto center = point_t{ui->canvas->width() / 2.0, ui->canvas->height() / 2.0};
  auto r = 10;
  int measure, max = 0;

  for (int teta = 0; teta <= 90; teta++) {
    auto line = line_t{.a = center,
                       .b = {center.x + r * cos(to_rads(teta)),
                             center.y + r * sin(to_rads(teta))}};
    arg.line = line;
    measure = request(*mediator, arg).execute({}, false);

    max = std::max(max, measure);
    series->append(teta, measure);
  }

  return max;
}

void MainWindow::on_compare_clicked() {
  auto *set0 = new QtCharts::QBarSet("Время без отрисовки");
  auto max = compare_methods(set0, false);

  auto *series = new QtCharts::QBarSeries();
  series->append(set0);

  auto *chart = new QtCharts::QChart();
  chart->addSeries(series);
  chart->setTitle("Сравнение алгоритмов");
  chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

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

void MainWindow::on_compare_steps_clicked() {
  auto round_to_10 = [](int n) -> int {
    int a = (n / 10) * 10;
    int b = a + 10;
    return ((n - a < b - n) ? a : b) + 10;
  };

  auto *chart = new QtCharts::QChart();
  chart->setTitle("Сравнение ступенчатости алгоритмов");
  chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

  auto *axisX = new QtCharts::QValueAxis();
  axisX->setTitleText("Угол");
  axisX->applyNiceNumbers();
  chart->addAxis(axisX, Qt::AlignBottom);

  auto *axisY = new QtCharts::QValueAxis();
  axisY->setTitleText("Кол-во ступенек");
  axisY->setLabelFormat("%d");
  axisY->applyNiceNumbers();
  chart->addAxis(axisY, Qt::AlignLeft);

  auto max = 0;
  for (int i = DDA; i < STD; ++i) {
    auto *series = new QtCharts::QLineSeries();

    max = std::max(max, compare_method_steps(i, series));
    series->setName(alg_titles[i - DDA]);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
  }

  axisY->setRange(0, round_to_10(max));

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

  ui->coords_apply->setEnabled(false);
  ui->bunch_apply->setEnabled(false);
}

void MainWindow::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  auto rcontent = ui->canvas->contentsRect();
  ui->canvas->scene()->setSceneRect(0, 0, rcontent.width(), rcontent.height());
}
