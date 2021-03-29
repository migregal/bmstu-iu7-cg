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
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow {
public:
  QWidget *centralwidget;
  QHBoxLayout *horizontalLayout;
  QGraphicsView *canvas;
  QFrame *frame;
  QVBoxLayout *verticalLayout;
  QGroupBox *groupBox_2;
  QVBoxLayout *verticalLayout_6;
  QRadioButton *circle_check;
  QRadioButton *ellipse_check;
  QGroupBox *groupBox;
  QVBoxLayout *verticalLayout_4;
  QRadioButton *black_check;
  QRadioButton *background_check;
  QFrame *method_frame;
  QVBoxLayout *verticalLayout_2;
  QLabel *method_label;
  QListView *method_list;
  QFrame *coords_frame;
  QGridLayout *gridLayout;
  QSpinBox *ra_spinbox;
  QPushButton *coords_apply;
  QSpinBox *xc_spinbox;
  QLabel *label_5;
  QSpinBox *rb_spinbox;
  QLabel *coords_label;
  QSpinBox *yc_spinbox;
  QLabel *label_4;
  QFrame *bunch_params;
  QGridLayout *gridLayout_2;
  QSpinBox *bunch_yc;
  QLabel *bunch_step_label;
  QSpinBox *bunch_xc;
  QSpinBox *bunch_step;
  QLabel *bunch_coord_label;
  QLabel *bunch_r_label;
  QSpinBox *bunch_r_a;
  QSpinBox *bunch_r_b;
  QPushButton *bunch_apply;
  QLabel *bunch_label;
  QSpinBox *bunch_count;
  QPushButton *cmp_apply;
  QPushButton *clear_screen_apply;
  QMenuBar *menubar;

  void setupUi(QMainWindow *MainWindow) {
    if (MainWindow->objectName().isEmpty())
      MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(837, 775);
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    horizontalLayout = new QHBoxLayout(centralwidget);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    canvas = new QGraphicsView(centralwidget);
    canvas->setObjectName(QString::fromUtf8("canvas"));
    QFont font;
    font.setPointSize(8);
    canvas->setFont(font);

    horizontalLayout->addWidget(canvas);

    frame = new QFrame(centralwidget);
    frame->setObjectName(QString::fromUtf8("frame"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy);
    frame->setFont(font);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    verticalLayout = new QVBoxLayout(frame);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    groupBox_2 = new QGroupBox(frame);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setFont(font);
    verticalLayout_6 = new QVBoxLayout(groupBox_2);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    circle_check = new QRadioButton(groupBox_2);
    circle_check->setObjectName(QString::fromUtf8("circle_check"));
    circle_check->setFont(font);

    verticalLayout_6->addWidget(circle_check);

    ellipse_check = new QRadioButton(groupBox_2);
    ellipse_check->setObjectName(QString::fromUtf8("ellipse_check"));
    ellipse_check->setFont(font);

    verticalLayout_6->addWidget(ellipse_check);

    verticalLayout->addWidget(groupBox_2);

    groupBox = new QGroupBox(frame);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    groupBox->setFont(font);
    groupBox->setCheckable(false);
    verticalLayout_4 = new QVBoxLayout(groupBox);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    black_check = new QRadioButton(groupBox);
    black_check->setObjectName(QString::fromUtf8("black_check"));
    black_check->setFont(font);
    black_check->setCheckable(true);
    black_check->setChecked(false);

    verticalLayout_4->addWidget(black_check);

    background_check = new QRadioButton(groupBox);
    background_check->setObjectName(QString::fromUtf8("background_check"));
    background_check->setFont(font);

    verticalLayout_4->addWidget(background_check);

    verticalLayout->addWidget(groupBox);

    method_frame = new QFrame(frame);
    method_frame->setObjectName(QString::fromUtf8("method_frame"));
    sizePolicy.setHeightForWidth(
        method_frame->sizePolicy().hasHeightForWidth());
    method_frame->setSizePolicy(sizePolicy);
    method_frame->setMinimumSize(QSize(0, 0));
    method_frame->setFont(font);
    method_frame->setFrameShape(QFrame::StyledPanel);
    method_frame->setFrameShadow(QFrame::Raised);
    verticalLayout_2 = new QVBoxLayout(method_frame);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    method_label = new QLabel(method_frame);
    method_label->setObjectName(QString::fromUtf8("method_label"));
    sizePolicy.setHeightForWidth(
        method_label->sizePolicy().hasHeightForWidth());
    method_label->setSizePolicy(sizePolicy);
    method_label->setFont(font);

    verticalLayout_2->addWidget(method_label);

    method_list = new QListView(method_frame);
    method_list->setObjectName(QString::fromUtf8("method_list"));
    sizePolicy.setHeightForWidth(method_list->sizePolicy().hasHeightForWidth());
    method_list->setSizePolicy(sizePolicy);
    method_list->setMinimumSize(QSize(200, 74));
    method_list->setFont(font);
    method_list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    method_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    method_list->setSizeAdjustPolicy(
        QAbstractScrollArea::AdjustToContentsOnFirstShow);

    verticalLayout_2->addWidget(method_list);

    verticalLayout->addWidget(method_frame);

    coords_frame = new QFrame(frame);
    coords_frame->setObjectName(QString::fromUtf8("coords_frame"));
    sizePolicy.setHeightForWidth(
        coords_frame->sizePolicy().hasHeightForWidth());
    coords_frame->setSizePolicy(sizePolicy);
    coords_frame->setFont(font);
    coords_frame->setFrameShape(QFrame::StyledPanel);
    coords_frame->setFrameShadow(QFrame::Raised);
    gridLayout = new QGridLayout(coords_frame);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    xc_spinbox = new QSpinBox(coords_frame);
    xc_spinbox->setObjectName(QString::fromUtf8("xc_spinbox"));
    xc_spinbox->setFont(font);
    xc_spinbox->setRange(0, 1000);

    gridLayout->addWidget(xc_spinbox, 2, 1, 1, 1);

    yc_spinbox = new QSpinBox(coords_frame);
    yc_spinbox->setObjectName(QString::fromUtf8("yc_spinbox"));
    yc_spinbox->setFont(font);
    yc_spinbox->setRange(0, 1000);

    gridLayout->addWidget(yc_spinbox, 2, 2, 1, 1);

    ra_spinbox = new QSpinBox(coords_frame);
    ra_spinbox->setObjectName(QString::fromUtf8("ra_spinbox"));
    ra_spinbox->setFont(font);
    ra_spinbox->setRange(0, 500);

    gridLayout->addWidget(ra_spinbox, 3, 1, 1, 1);

    rb_spinbox = new QSpinBox(coords_frame);
    rb_spinbox->setObjectName(QString::fromUtf8("rb_spinbox"));
    rb_spinbox->setFont(font);
    rb_spinbox->setEnabled(false);
    rb_spinbox->setRange(0, 500);

    gridLayout->addWidget(rb_spinbox, 3, 2, 1, 1);

    coords_apply = new QPushButton(coords_frame);
    coords_apply->setObjectName(QString::fromUtf8("coords_apply"));
    coords_apply->setEnabled(false);
    coords_apply->setFont(font);

    gridLayout->addWidget(coords_apply, 4, 0, 1, 3);

    label_5 = new QLabel(coords_frame);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setFont(font);

    gridLayout->addWidget(label_5, 3, 0, 1, 1);

    coords_label = new QLabel(coords_frame);
    coords_label->setObjectName(QString::fromUtf8("coords_label"));
    sizePolicy.setHeightForWidth(
        coords_label->sizePolicy().hasHeightForWidth());
    coords_label->setSizePolicy(sizePolicy);
    coords_label->setFont(font);

    gridLayout->addWidget(coords_label, 0, 0, 1, 3);

    label_4 = new QLabel(coords_frame);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setFont(font);

    gridLayout->addWidget(label_4, 2, 0, 1, 1);

    verticalLayout->addWidget(coords_frame);

    bunch_params = new QFrame(frame);
    bunch_params->setObjectName(QString::fromUtf8("bunch_params"));
    bunch_params->setFont(font);
    bunch_params->setFrameShape(QFrame::StyledPanel);
    bunch_params->setFrameShadow(QFrame::Raised);
    gridLayout_2 = new QGridLayout(bunch_params);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));

    bunch_xc = new QSpinBox(bunch_params);
    bunch_xc->setObjectName(QString::fromUtf8("bunch_xc"));
    bunch_xc->setFont(font);
    bunch_xc->setRange(0, 1000);

    gridLayout_2->addWidget(bunch_xc, 2, 1, 1, 1);

    bunch_yc = new QSpinBox(bunch_params);
    bunch_yc->setObjectName(QString::fromUtf8("bunch_yc"));
    bunch_yc->setFont(font);
    bunch_yc->setRange(0, 1000);

    gridLayout_2->addWidget(bunch_yc, 2, 2, 1, 1);

    bunch_r_label = new QLabel(bunch_params);
    bunch_r_label->setObjectName(QString::fromUtf8("bunch_r_label"));
    bunch_r_label->setFont(font);

    gridLayout_2->addWidget(bunch_r_label, 3, 0, 1, 1);

    bunch_r_a = new QSpinBox(bunch_params);
    bunch_r_a->setObjectName(QString::fromUtf8("bunch_r_a"));
    bunch_r_a->setFont(font);

    gridLayout_2->addWidget(bunch_r_a, 3, 1, 1, 1);

    bunch_r_b = new QSpinBox(bunch_params);
    bunch_r_b->setObjectName(QString::fromUtf8("bunch_r_b"));
    bunch_r_b->setFont(font);
    bunch_r_b->setEnabled(false);

    gridLayout_2->addWidget(bunch_r_b, 3, 2, 1, 1);

    bunch_step_label = new QLabel(bunch_params);
    bunch_step_label->setObjectName(QString::fromUtf8("bunch_setp_label"));
    bunch_step_label->setFont(font);

    gridLayout_2->addWidget(bunch_step_label, 4, 0, 1, 1);

    bunch_step = new QSpinBox(bunch_params);
    bunch_step->setObjectName(QString::fromUtf8("bunch_step"));
    bunch_step->setFont(font);

    gridLayout_2->addWidget(bunch_step, 4, 1, 1, 1);

    bunch_count = new QSpinBox(bunch_params);
    bunch_count->setObjectName(QString::fromUtf8("bunch_count"));
    bunch_count->setFont(font);

    gridLayout_2->addWidget(bunch_count, 4, 2, 1, 1);

    bunch_coord_label = new QLabel(bunch_params);
    bunch_coord_label->setObjectName(QString::fromUtf8("bunch_coord_label"));
    bunch_coord_label->setFont(font);

    gridLayout_2->addWidget(bunch_coord_label, 2, 0, 1, 1);

    bunch_apply = new QPushButton(bunch_params);
    bunch_apply->setObjectName(QString::fromUtf8("bunch_apply"));
    bunch_apply->setEnabled(false);
    bunch_apply->setFont(font);

    gridLayout_2->addWidget(bunch_apply, 5, 0, 1, 3);

    bunch_label = new QLabel(bunch_params);
    bunch_label->setObjectName(QString::fromUtf8("bunch_label"));
    bunch_label->setFont(font);

    gridLayout_2->addWidget(bunch_label, 0, 0, 1, 3);

    verticalLayout->addWidget(bunch_params);

    cmp_apply = new QPushButton(frame);
    cmp_apply->setObjectName(QString::fromUtf8("cmp_apply"));
    cmp_apply->setFont(font);

    verticalLayout->addWidget(cmp_apply);

    clear_screen_apply = new QPushButton(frame);
    clear_screen_apply->setObjectName(QString::fromUtf8("clear_screen_apply"));
    clear_screen_apply->setFont(font);

    verticalLayout->addWidget(clear_screen_apply);

    horizontalLayout->addWidget(frame, 0, Qt::AlignTop);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 837, 23));
    MainWindow->setMenuBar(menubar);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
  } // setupUi

  void retranslateUi(QMainWindow *MainWindow) {
    MainWindow->setWindowTitle(
        QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    groupBox_2->setTitle(
        QCoreApplication::translate("MainWindow",
                                    "\320\244\320\270\320\263\321\203\321\200"
                                    "\320\260 \320\264\320\273\321\217 "
                                    "\320\277\320\276\321\201\321\202\321\200"
                                    "\320\276\320\265\320\275\320\270\321\217",
                                    nullptr));
    circle_check->setText(
        QCoreApplication::translate("MainWindow",
                                    "\320\236\320\272\321\200\321\203\320\266"
                                    "\320\275\320\276\321\201\321\202\321\214",
                                    nullptr));
    ellipse_check->setText(QCoreApplication::translate(
        "MainWindow", "\320\255\320\273\320\273\320\270\320\277\321\201",
        nullptr));
    groupBox->setTitle(
        QCoreApplication::translate("MainWindow",
                                    "\320\246\320\262\320\265\321\202 "
                                    "\320\277\320\276\321\201\321\202\321\200"
                                    "\320\276\320\265\320\275\320\270\321\217",
                                    nullptr));
    black_check->setText(QCoreApplication::translate(
        "MainWindow", "\320\247\320\265\321\200\320\275\321\213\320\271",
        nullptr));
    background_check->setText(QCoreApplication::translate(
        "MainWindow",
        "\320\246\320\262\320\265\321\202 \321\204\320\276\320\275\320\260",
        nullptr));
    method_label->setText(
        QCoreApplication::translate("MainWindow",
                                    "\320\234\320\265\321\202\320\276\320\264 "
                                    "\320\277\320\276\321\201\321\202\321\200"
                                    "\320\276\320\265\320\275\320\270\321\217",
                                    nullptr));
    coords_apply->setText(QCoreApplication::translate(
        "MainWindow",
        "\320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321"
        "\214 \321\204\320\270\320\263\321\203\321\200\321\203",
        nullptr));
    label_5->setText(
        QCoreApplication::translate("MainWindow", "Ra, Rb", nullptr));
    coords_label->setText(QCoreApplication::translate(
        "MainWindow",
        "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321"
        "\213 \321\204\320\270\320\263\321\203\321\200\321\213",
        nullptr));
    label_4->setText(QCoreApplication::translate(
        "MainWindow", "(\321\205\321\201, \321\203\321\201)", nullptr));
    bunch_step_label->setText(QCoreApplication::translate(
        "MainWindow", "\320\250\320\260\320\263, N", nullptr));
    bunch_coord_label->setText(QCoreApplication::translate(
        "MainWindow", "(\321\205\321\201, \321\203\321\201)", nullptr));
    bunch_r_label->setText(
        QCoreApplication::translate("MainWindow", "Ra, Rb", nullptr));
    bunch_apply->setText(QCoreApplication::translate(
        "MainWindow",
        "\320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321"
        "\214 \321\201\320\277\320\265\320\272\321\202\321\200",
        nullptr));
    bunch_label->setText(QCoreApplication::translate(
        "MainWindow",
        "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321"
        "\213 \321\201\320\277\320\265\320\272\321\202\321\200\320\260",
        nullptr));
    cmp_apply->setText(
        QCoreApplication::translate("MainWindow",
                                    "\320\241\321\200\320\260\320\262\320\275"
                                    "\320\265\320\275\320\270\320\265 "
                                    "\320\260\320\273\320\263\320\276\321\200"
                                    "\320\270\321\202\320\274\320\276\320\262",
                                    nullptr));
    clear_screen_apply->setText(QCoreApplication::translate(
        "MainWindow",
        "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 "
        "\321\215\320\272\321\200\320\260\320\275",
        nullptr));
  } // retranslateUi
};

namespace Ui {
class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGN_H
