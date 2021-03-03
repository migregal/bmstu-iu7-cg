/********************************************************************************
** Form generated from reading UI file 'design.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGN_H
#define DESIGN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QTableView>
#include <QLabel>
#include <QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_MainWindow {
public:
    QWidget *root;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QTableView *pointsList;
    QLabel *label;
    QSpinBox *pointsCount;
    QHBoxLayout *coordInputLayout;
    QPushButton *triangleSearchBtn;
    QPushButton *clearPointsBtn;
    QFrame *canvasPlaceholder;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    virtual void setupUi(QMainWindow *MainWindow);

    void retranslateUi(QMainWindow *MainWindow) const;
};

QT_END_NAMESPACE

#endif // DESIGN_H
