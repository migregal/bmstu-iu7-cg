//
// Created by gregory on 12.02.2021.
//

#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QStyledItemDelegate>

#include "MainWindow.h"
#include "utils/ItemDelegate.h"
#include "widgets/Canvas.h"

inline void incorrectPointInputData(int idx) {
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("Ошибка");
    msg.setText(QString("Ошибка в записи точки № %1\n").arg(idx));
    msg.exec();
}

inline void incorrectPointsListSize() {
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("Ошибка");
    msg.setText("Слишком мало точек\n");
    msg.exec();
}

inline void incorrectPointInputCount(int idx, float x, float y) {
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("Ошибка");
    msg.setText(
            QString("Точка №%1 (%2, %3) указана более 1 раза\n")
                    .arg(idx)
                    .arg(x)
                    .arg(y));
    msg.exec();
}

inline QString rnd(float a) {
    return QString::number(a, 'f', 2);
}

#define success_txt(_a_, _b_, _c_, _rs_) \
QString("Искомый треугольник построен\n"\
"Вершины:\n  A(%1) (%2; %3)\n  B(%4) (%5; %6)\n  C(%7) (%8; %9)\n"\
"Площадь вписанной и описанной окружностей, "\
"соответственно: \n  %10 ед. кв.\n  %11 ед. кв\n"\
"Результат %12").\
arg(_a_.first).arg(rnd(_a_.second.x())).arg(rnd(_a_.second.y())).\
arg(_b_.first).arg(rnd(_b_.second.x())).arg(rnd(_b_.second.y())).\
arg(_c_.first).arg(rnd(_c_.second.x())).arg(rnd(_c_.second.y()))\
.arg(rnd(_rs_.first)).arg(rnd(_rs_.second)).arg(rnd(_rs_.second - _rs_.first))

inline void successMsg(full_detailed data) {
    tr_detailed info;
    std::pair<float, float> Rs;
    std::tie(info, Rs) = data;

    std::pair<int, QPointF> a, b, c;
    std::tie(a, b, c) = info;

    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle("Успех");
    msg.setText(success_txt(a, b, c, Rs));
    msg.exec();
}

inline void unresolvableProblem() {
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("Ошибка");
    msg.setText(
            "Точки лежат на одной прямой\n"
            "Построить треугольник невозможно");
    msg.exec();
}

Ui::MainWindow::MainWindow() {
    Ui_MainWindow::setupUi(this);

    tableDataModel = new QStandardItemModel(0, 2, pointsList);
    pointsList->setModel(tableDataModel);

    auto *layout = new QVBoxLayout();
    canvasPlaceholder->setLayout(layout);
    canvas = new Canvas();
    layout->addWidget(canvas);


    connect(pointsCount, qOverload<int>(&QSpinBox::valueChanged), this,
            &MainWindow::on_count_changed);

    connect(pointsList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &MainWindow::slot_selection_changed
    );

    connect(triangleSearchBtn, &QPushButton::clicked, this,
            &MainWindow::on_search_triangle_button_clicked);

    connect(clearPointsBtn, &QPushButton::clicked, this,
            &MainWindow::on_delete_button_clicked);
}

void Ui::MainWindow::slot_selection_changed(const QItemSelection &oSelected,
                                            const QItemSelection &oDeselected) {
    const int iSelectedItems =
            pointsList->selectionModel()->selectedIndexes().size();

    clearPointsBtn->setEnabled((iSelectedItems > 0));
}

void Ui::MainWindow::on_count_changed(int count) {
    if (count >= 3) {
        update_points_list(count);
        return;
    }

    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("Ошибка");
    msg.setText("Слишком мало точек\n");
    msg.exec();
}

void Ui::MainWindow::update_points_list(int count) {
    if (count < 3) {
        incorrectPointsListSize();
        return;
    }

    pointsList->setModel(tableDataModel);
    pointsList->setItemDelegate(new ItemDelegate);

    if (count > tableDataModel->rowCount())
        tableDataModel->insertRows(tableDataModel->rowCount(),
                                   count - tableDataModel->rowCount());
    else
        tableDataModel->removeRows(count, tableDataModel->rowCount() - count);
}

bool checked_convertion(QAbstractItemModel *m, int idx, float &x, float &y) {
    bool converted_x = true, converted_y = true;
    auto tx = m->data(m->index(idx, 0)).toFloat(&converted_x);
    auto ty = m->data(m->index(idx, 1)).toFloat(&converted_y);

    if (!converted_x || !converted_y)
        return false;

    x = tx;
    y = ty;
    return true;
}

void Ui::MainWindow::on_search_triangle_button_clicked() {
    canvas->clearPoints();

    for (int i = 0; i < pointsList->model()->rowCount(); i++) {
        float x, y;
        if (!checked_convertion(pointsList->model(), i, x, y)) {
            incorrectPointInputData(i + 1);
            canvas->clearPoints();
            canvas->update();
            return;
        }

        if (canvas->addPoint(i + 1, x, y)) continue;

        incorrectPointInputCount(i + 1, x, y);
        canvas->clearPoints();
        canvas->update();
        return;
    }

    if (!canvas->buildTriangle()) {
        unresolvableProblem();
        canvas->clearPoints();
        canvas->update();
        return;
    }

    canvas->update();
    successMsg(canvas->get_triangle_info());
}

void Ui::MainWindow::on_delete_button_clicked() {
    auto model = pointsList->selectionModel();
    if (!model->hasSelection())
        return;

    auto rows = model->selectedRows();
    for (auto row = rows.rbegin(); row != rows.rend(); ++row) {
        pointsList->model()->removeRow(row->row());
    }

    pointsCount->setValue(pointsList->model()->rowCount());
    if (pointsList->model()->rowCount() >= 3)
        on_search_triangle_button_clicked();
    else {
        canvas->clearPoints();
        canvas->update();
        incorrectPointsListSize();
    }
}

