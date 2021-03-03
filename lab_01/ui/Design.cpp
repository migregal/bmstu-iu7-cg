//
// Created by gregory on 11.02.2021.
//

#include <QHeaderView>

#include "widgets/Canvas.h"
#include "Design.h"

void Ui_MainWindow::setupUi(QMainWindow *MainWindow) {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(698, 591);
    root = new QWidget(MainWindow);
    root->setObjectName(QString::fromUtf8("root"));
    horizontalLayout = new QHBoxLayout(root);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    pointsList = new QTableView(root);
    pointsList->setObjectName(QString::fromUtf8("pointsList"));
    pointsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(pointsList->sizePolicy().hasHeightForWidth());
    pointsList->setSizePolicy(sizePolicy);
    pointsList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pointsList->setHorizontalScrollBarPolicy(
            Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    verticalLayout->addWidget(pointsList);

    label = new QLabel(root);
    label->setObjectName(QString::fromUtf8("label"));

    verticalLayout->addWidget(label);

    pointsCount = new QSpinBox(root);
    pointsCount->setObjectName(QString::fromUtf8("pointsCount"));
    QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(
            pointsCount->sizePolicy().hasHeightForWidth());
    pointsCount->setSizePolicy(sizePolicy1);
    verticalLayout->addWidget(pointsCount);

    coordInputLayout = new QHBoxLayout();
    coordInputLayout->setObjectName(QString::fromUtf8("coordInputLayout"));

    verticalLayout->addLayout(coordInputLayout);

    triangleSearchBtn = new QPushButton(root);
    triangleSearchBtn->setObjectName(QString::fromUtf8("triangleSearchBtn"));

    verticalLayout->addWidget(triangleSearchBtn);

    clearPointsBtn = new QPushButton(root);
    clearPointsBtn->setObjectName(QString::fromUtf8("clearPointsBtn"));
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(
            clearPointsBtn->sizePolicy().hasHeightForWidth());
    clearPointsBtn->setSizePolicy(sizePolicy2);
    clearPointsBtn->setEnabled(false);

    verticalLayout->addWidget(clearPointsBtn);


    horizontalLayout->addLayout(verticalLayout);

    canvasPlaceholder = new QFrame(root);
    canvasPlaceholder->setObjectName(QString::fromUtf8("canvasPlaceholder"));
    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(
            canvasPlaceholder->sizePolicy().hasHeightForWidth());
    canvasPlaceholder->setSizePolicy(sizePolicy3);
    canvasPlaceholder->setFrameShape(QFrame::StyledPanel);
    canvasPlaceholder->setFrameShadow(QFrame::Raised);

    horizontalLayout->addWidget(canvasPlaceholder);

    MainWindow->setCentralWidget(root);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 698, 23));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
}

void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow) const {
    MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\233\320\260\320\261\320\276\321\200\320\260\321\202\320\276\321\200\320\275\320\260\321\217 \321\200\320\260\320\261\320\276\321\202\320\260 \342\204\2261", nullptr));
    label->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273-\320\262\320\276 \321\202\320\276\321\207\320\265\320\272", nullptr));
    triangleSearchBtn->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\320\262\320\265\321\201\321\202\320\270 \321\200\320\260\321\201\321\207\320\265\321\202", nullptr));
    clearPointsBtn->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
} // retranslateUi