//
// Created by gregory on 12.02.2021.
//

#ifndef LAB_01_MAINWINDOW_H
#define LAB_01_MAINWINDOW_H

#include <QStandardItemModel>

#include "Design.h"
#include "widgets/Canvas.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow : public QMainWindow, Ui_MainWindow {
    Q_OBJECT

        QStandardItemModel *tableDataModel;
        Canvas *canvas;

        void update_points_list(int count);

    public:
        explicit MainWindow();

        void on_count_changed(int count);

        void on_search_triangle_button_clicked();

        void on_delete_button_clicked();

        void slot_selection_changed(const QItemSelection &oSelected,
                               const QItemSelection &oDeselected);
    };
}

QT_END_NAMESPACE

#endif //LAB_01_MAINWINDOW_H
