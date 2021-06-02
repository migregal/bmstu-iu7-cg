import sys

from copy import deepcopy

from PyQt5 import QtWidgets
from PyQt5 import uic
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

from funcs import *
from simple_cut import simple_cut


# Класс главного окна
class Window(QMainWindow):
    def __init__(self):
        QWidget.__init__(self)

        # Загрузка интерфейса
        uic.loadUi("design.ui", self)

        # Переменные
        self.bg_color = QColor(Qt.white)
        self.line_color = QColor(Qt.black)
        self.cutter_color = QColor(Qt.green)
        self.cut_line_color = QColor(Qt.red)

        self.ctrl_pressed = False

        self.lines: list[Line] = []
        self.colored_lines = []
        self.cur_line = []
        self.follow_line = None

        self.cutter: Cutter = None
        self.drawing_cutter = False
        self.cur_cutter = []
        self.follow_cutter = None

        # Добавляем полотно
        self.scene = MyScene(self, 0, 0, 1920, 1080)
        self.mainview.setScene(self.scene)
        self.pen: QPen = QPen()

        self.mainview.ensureVisible(0, 0, 0, 0)
        self.mainview.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.mainview.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.mainview.setRenderHint(QPainter.Antialiasing)

        self.but_add_line.clicked.connect(self.get_line)
        self.but_add_cutter.clicked.connect(self.get_cutter)
        self.but_cut.clicked.connect(self.cut)
        self.but_clear.clicked.connect(self.clear)

        # Остальные настройки
        self.mainview.setMouseTracking(True)
        self.mainview.viewport().installEventFilter(self)

    # Отслеживание передвижения мыши
    def eventFilter(self, src, event):
        if event.type() == QEvent.MouseMove and src is self.mainview.viewport():
            x = event.x()
            y = event.y()

            following_line(self, x, y)
            following_cutter(self, x, y)

        return QWidget.eventFilter(self, src, event)

    def keyPressEvent(self, event):
        key = event.key()
        if key == Qt.Key_Shift:
            self.ctrl_pressed = True

    def keyReleaseEvent(self, event):
        key = event.key()
        if key == Qt.Key_Shift:
            self.ctrl_pressed = False

    def get_line(self):
        add_line(
            self,
            self.inp_x1.value(),
            self.inp_y1.value(),
            self.inp_x2.value(),
            self.inp_y2.value(),
            self.line_color)

    def get_cutter(self):
        self.scene.removeItem(self.follow_cutter)
        self.drawing_cutter = False
        del_cutter(self)
        add_cutter(
            self,
            self.inp_x_left.value(),
            self.inp_y_up.value(),
            self.inp_x_right.value(),
            self.inp_y_down.value(),
            self.cutter_color
        )

    def cut(self):
        if self.cutter:
            for i in self.colored_lines:
                try:
                    self.scene.removeItem(i)
                except BaseException:
                    pass

            for line in self.lines:
                visible, p1, p2 = simple_cut(self.cutter.rect,
                                             deepcopy(line.line))

                if visible:
                    self.pen.setWidth(2)
                    draw_line(self, p1, p2, self.cut_line_color)
                    self.pen.setWidth(1)

    def draw_apart_line(self, line: QLine, p1:QPoint, p2:QPoint):
        print(line)
        if p1 == line.p1() and p2 == line.p2():
            return

        if p1 == line.p1():
            draw_line(self, line.p2(), p2, self.line_color)
        elif p1 == line.p2():
            draw_line(self, line.p1(), p2, self.line_color)
        elif p2 == line.p1():
            draw_line(self, line.p1(), p2, self.line_color)
        elif p2 == line.p2():
            draw_line(self, line.p1(), p1, self.line_color)
        else:
            draw_line(self, line.p1(), line.p2(), self.line_color)

    def clear(self):
        self.scene.clear()

        self.lines.clear()
        self.cur_line.clear()
        self.follow_line = None

        self.cutter = None
        self.cur_cutter.clear()
        self.follow_cutter = None
        self.drawing_cutter = False


class MyScene(QtWidgets.QGraphicsScene):
    def __init__(self, win: Window, *args):
        super().__init__(*args)
        self.window = win

        self.last_x = None
        self.last_y = None

    def mousePressEvent(self,
                        event: QtWidgets.QGraphicsSceneMouseEvent) -> None:
        if event.buttons() == Qt.LeftButton:
            if not self.window.drawing_cutter:
                line_on_screen(self.window, event.scenePos().x(),
                               event.scenePos().y())
            else:
                cutter_on_screen(self.window, event.scenePos().x(),
                                 event.scenePos().y())

        elif event.buttons() == Qt.RightButton:
            self.window.drawing_cutter = True
            del_cutter(self.window)
            cutter_on_screen(self.window, event.scenePos().x(),
                             event.scenePos().y())


if __name__ == '__main__':
    app = QApplication([])
    application = Window()
    application.show()

    sys.exit(app.exec())
