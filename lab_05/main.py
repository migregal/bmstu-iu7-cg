from PyQt5 import QtWidgets, uic
from PyQt5.QtWidgets import QTableWidgetItem, QHeaderView, QSizePolicy, QColorDialog, QMessageBox
from PyQt5.QtGui import QPen, QColor, QColorConstants, QImage, QPixmap, QPainter
from PyQt5.QtCore import Qt, QTime, QCoreApplication, QEventLoop, QPoint
from time import perf_counter

col_one = Qt.black
col_zero = Qt.white


class Window(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        uic.loadUi("window.ui", self)

        self.edges = []
        self.edges_count = 0
        self.point_now = None
        self.point_lock = None

        self.color = QColorConstants.White
        self.backGroundColor = QColorConstants.Black

        self.scene = myScene(self, 0, 0, 1920, 1280)

        self.view.setScene(self.scene)
        self.image = QImage(1920, 1280, QImage.Format_ARGB32_Premultiplied)
        self.image.fill(col_zero)
        self.lock.clicked.connect(self.__lock)
        self.erase.clicked.connect(self.__clean_all)
        self.paint.clicked.connect(self.__fill_xor)
        self.add_point_btn.clicked.connect(self.__add_point)

        self.pen = QPen(col_one)
        self.delay.setChecked(False)
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.choose_color.clicked.connect(self.__chooseColor)

        scene = QtWidgets.QGraphicsScene(0, 0, 68, 20)

        image = QPixmap(68, 20)
        image.fill(self.backGroundColor)

        scene.addPixmap(image)
        self.choosed_color.setScene(scene)

    def __chooseColor(self):
        self.backGroundColor = QColorDialog.getColor()

        if self.color.isValid():
            scene = QtWidgets.QGraphicsScene(0, 0, 68, 20)

            image = QPixmap(68, 20)
            image.fill(self.backGroundColor)

            scene.addPixmap(image)
            self.choosed_color.setScene(scene)

    def add_row(self):
        self.table.insertRow(self.table.rowCount())


    def __add_point(self):

        posx = self.x_spinbox.value()
        posy = self.y_spinbox.value()

        self.add_point(QPoint(posx, posy))

    def add_point(self, point):
        global w

        self.edges_count += 1
        if self.point_now is None:
            self.point_now = point
            self.point_lock = point

            self.add_row()

            i = self.table.rowCount() - 1
            item_x = QTableWidgetItem("{0}".format(point.x()))
            item_y = QTableWidgetItem("{0}".format(point.y()))
            self.table.setItem(i, 0, item_x)
            self.table.setItem(i, 1, item_y)
        else:
            self.edges.append([self.point_now.x(), self.point_now.y(),
                            point.x(), point.y()])
            self.point_now = point
            self.add_row()
            i = self.table.rowCount() - 1
            item_x = QTableWidgetItem("{0}".format(point.x()))
            item_y = QTableWidgetItem("{0}".format(point.y()))
            self.table.setItem(i, 0, item_x)
            self.table.setItem(i, 1, item_y)
            item_x = self.table.item(i - 1, 0)
            item_y = self.table.item(i - 1, 1)
            self.scene.addLine(point.x(), point.y(), float(
                item_x.text()), float(item_y.text()), self.pen)

    def __lock(self):
        if (self.edges_count < 3):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Warning)

            msg.setWindowTitle("Внимание")
            msg.setText("Слишком мало точек. Нельзя замкнуть фигуру")

            msg.setStandardButtons(QMessageBox.Ok)

            msg.exec_()
            return

        self.edges_count = 0
        self.edges.append([self.point_now.x(), self.point_now.y(),
                           self.point_lock.x(), self.point_lock.y()])
        self.scene.addLine(self.point_now.x(), self.point_now.y(),
        self.point_lock.x(), self.point_lock.y(), self.pen)
        self.point_now = None

    def __clean_all(self):
        self.scene.clear()
        self.table.clear()
        self.edges = []
        self.point_now = None
        self.point_lock = None
        self.image.fill(col_zero)
        r = self.table.rowCount()
        for i in range(r, -1, -1):
            self.table.removeRow(i)

    def __draw_edges(self, image, edges):
        pix = QPixmap()
        painter = QPainter()

        painter.begin(self.image)

        pen = QPen(QColorConstants.Black)
        # pen.setWidth(2)
        painter.setPen(pen)

        for ed in self.edges:
            painter.drawLine(int(ed[0]), int(ed[1]), int(ed[2]), int(ed[3]))

        painter.end()

        pix.convertFromImage(self.image)
        self.scene.clear()
        self.scene.addPixmap(pix)

    def __delay(self):
        QtWidgets.QApplication.processEvents(QEventLoop.AllEvents, 1)

    def __find_max_x(self, ed):
        x_max = None
        for i in range(len(ed)):
            if x_max is None or ed[i][0] > x_max:
                x_max = ed[i][0]

            if x_max is None or ed[i][2] > x_max:
                x_max = ed[i][2]

        return x_max

    def __fill_xor(self):

        if (self.edges_count != 0):
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Warning)

            msg.setWindowTitle("Внимание")
            msg.setText("Похоже, вы не замкнули какую-то фигуру")

            msg.setStandardButtons(QMessageBox.Ok)

            msg.exec_()
            return

        pix = QPixmap()
        p = QPainter()


        start = perf_counter()
        xm = self.__find_max_x(self.edges)
        for ed in self.edges:
            p.begin(self.image)
            # если горизонтальное ребро - дальше
            if ed[1] == ed[3]:  #т1 должен быть выше, чем т2
                continue
            # иначе определяем границы сканирования
            if ed[1] > ed[3]:
                ed[1], ed[3] = ed[3], ed[1]
                ed[0], ed[2] = ed[2], ed[0]

            y, end_y = ed[1], ed[3]
            dx = (ed[2] - ed[0]) / (ed[3] - ed[1])
            start_x = ed[0]

            while y < end_y:
                # определяем пересечение
                x = start_x
                while x < xm:
                    col = QColor(self.image.pixel(int(x), int(y)))
                    if col == self.backGroundColor:
                        p.setPen(QPen(self.color))
                    else:
                        p.setPen(QPen(self.backGroundColor))
                    p.drawPoint(int(x), int(y))
                    x += 1

                start_x += dx
                y += 1

                if self.delay.isChecked():
                    self.__delay()
                    pix.convertFromImage(self.image)
                    self.scene.addPixmap(pix)
                    self.time_label.setText(f'Время:')

            end = perf_counter()

            if not self.delay.isChecked():
                pix.convertFromImage(self.image)
                self.scene.addPixmap(pix)

                self.time_label.setText(f'Время: {end - start:.3}')

            p.end()

        self.__draw_edges(self.image, self.edges)


    def __add_point_by_btn(self):
        x = self.x.value()
        y = self.y.value()
        p = QPoint()
        p.setX(x)
        p.setY(y)
        add_point(p)


class myScene(QtWidgets.QGraphicsScene):
    def __init__(self, win: Window, *args):
        super().__init__(*args)
        self.window = win

        self.last_x = None
        self.last_y = None

    def mousePressEvent(self, event: QtWidgets.QGraphicsSceneMouseEvent) -> None:
        if event.buttons() == Qt.LeftButton:
            posx = int(event.scenePos().x())
            posy = int(event.scenePos().y())

            if event.modifiers() & Qt.ShiftModifier and self.last_x:
                if posy != self.last_y:
                    der = (posx - self.last_x) / (posy - self.last_y)
                else:
                    der = 2

                if abs(der) <= 1:
                    posx = self.last_x
                else:
                    posy = self.last_y

            self.window.add_point(QPoint(posx, posy))

            self.last_x = posx
            self.last_y = posy


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    Window().show()
    sys.exit(app.exec_())
