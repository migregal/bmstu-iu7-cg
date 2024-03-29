#include <MainWindow.h>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QStyle>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  MainWindow window;
  window.setGeometry(QStyle::alignedRect(
      Qt::LeftToRight, Qt::AlignCenter, window.size(),
      QGuiApplication::primaryScreen()->availableGeometry()));
  window.show();

  return QApplication::exec();
}
