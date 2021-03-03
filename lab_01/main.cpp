#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>

#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle("Условие задачи");
    msg.setText("На плоскости дано множество точек.\n"
                "Найти такой треугольник, с вершинами в этих точках, "
                "для которого разность площадей описанного и "
                "вписанного кругов максимальна.");
    msg.exec();

    Ui::MainWindow window;
    window.setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    window.size(),
                    QGuiApplication::primaryScreen()->availableGeometry()
            ));
    window.show();

    return QApplication::exec();
}
