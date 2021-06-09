import sys
from numpy import sign

from time import time
from copy import deepcopy
from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5 import QtWidgets, uic
from PyQt5.QtWidgets import QRadioButton, QTableWidgetItem, QMessageBox, QColorDialog
from PyQt5.QtGui import QBrush, QColorConstants, QPen, QColor, QImage, QPolygonF
from PyQt5.QtCore import Qt, QPoint, QLine

now = None
end_cutter_ = False
end_polygon_ = False
ctrl = False
wind = None


class Scene(QtWidgets.QGraphicsScene):
    def keyPressEvent(self, event):
        global ctrl

        if event.key() == Qt.Key_Control:
            ctrl = not ctrl

    # добавить точку по щелчку мыши
    def mousePressEvent(self, QMouseEvent):
        global ctrl

        if (QMouseEvent.button() == Qt.LeftButton):
            add_point(QMouseEvent.scenePos())

        if (QMouseEvent.button() == Qt.RightButton):
            end_cutter()
            end_polygon()

        ctrl = False


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        uic.loadUi("win.ui", self)

        self.graphicsView.scale(1, 1)

        self.scene = Scene(0, 0, 1366, 768)
        self.scene.win = self
        self.graphicsView.setScene(self.scene)
        self.image = QImage(1366, 768, QImage.Format_ARGB32_Premultiplied)
        self.image.fill(Qt.white)

        self.pen_cutter = QtGui.QPen(QColorConstants.Green)
        self.pen_cutter.setWidth(0)
        self.pen_polygon = QtGui.QPen(QColorConstants.Blue)
        self.pen_polygon.setWidth(0)
        self.pen_res = QtGui.QPen(QColorConstants.Red)
        self.pen_res.setWidth(0)
        self.input_cutter = True
        self.input_polygon = False
        self.polygon = []
        self.cutter = []
        self.clip = None
        self.point_now = None
        self.color_back = QtCore.Qt.white

        self.cutter_color.clicked.connect(lambda: self.__get_cutter_color())
        self.cut_line_color.clicked.connect(
            lambda: self.__get_cut_line_color())

        self.close_cutter.clicked.connect(lambda: end_cutter())
        self.close_polygon.clicked.connect(lambda: end_polygon())

        self.polygon_color.clicked.connect(lambda: self.__get_polygon_color())

        self.draw_polygon.clicked.connect(self.chang)
        self.draw_cutter.clicked.connect(self.chang)

        self.clear_screen.clicked.connect(self.__clear_screen)
        self.cut_lines.clicked.connect(sazerland_hodjmen_alg)

        self.add_polygon_vertice.clicked.connect(self.add_polygon)
        self.add_cutter_vertice.clicked.connect(self.add_cutter)

    def __get_cutter_color(self):
        color = QColorDialog.getColor()

        if not QColor.isValid(color):
            return

        self.pen_cutter.setColor(color)
        hexcolor = color.name()
        self.cutter_color.setStyleSheet('QPushButton{background-color:' +
                                        hexcolor + '}')

    def __get_cut_line_color(self):
        color = QColorDialog.getColor()

        if not QColor.isValid(color):
            return

        self.pen_res.setColor(color)
        hexcolor = color.name()
        self.cut_line_color.setStyleSheet('QPushButton{background-color:' +
                                          hexcolor + '}')

    def __get_polygon_color(self):
        color = QColorDialog.getColor()

        if not QColor.isValid(color):
            return

        self.pen_polygon.setColor(color)
        hexcolor = color.name()
        self.polygon_color.setStyleSheet('QPushButton{background-color:' +
                                      hexcolor + '}')

    def chang(self):
        global now, now_buf
        if self.draw_polygon.isChecked():
            now_buf = now
            now = None
            self.input_polygon = True
            self.input_cutter = False
        else:
            self.input_polygon = False
            self.input_cutter = True

    def __clear_screen(self):
        global now
        self.scene.clear()
        self.polygon = []
        self.cutter = []
        now = None
        self.image.fill(Qt.white)
        r = self.table_polygon.rowCount()
        for i in range(r, -1, -1):
            self.table_polygon.removeRow(i)
        r = self.table_cutter.rowCount()
        for i in range(r, -1, -1):
            self.table_cutter.removeRow(i)

    def set_black_bg(self):
        self.graphicsView.setStyleSheet("background-color: black")
        self.color_back = QtCore.Qt.black

    def set_white_bg(self):
        self.graphicsView.setStyleSheet("background-color: white")
        self.color_back = QtCore.Qt.white

    def set_blue_bg(self):
        self.graphicsView.setStyleSheet("background-color: blue")
        self.color_back = QtCore.Qt.blue

    def set_red_bg(self):
        self.graphicsView.setStyleSheet("background-color: red")
        self.color_back = QtCore.Qt.red

    def set_green_bg(self):
        self.graphicsView.setStyleSheet("background-color: #00ff00")
        self.color_back = QtCore.Qt.green

    def set_yellow_bg(self):
        self.graphicsView.setStyleSheet("background-color: yellow")
        self.color_back = QtCore.Qt.yellow

    def add_cutter(self):
        x = int(self.cutter_x.value())
        y = int(self.cutter_y.value())

        add_point(QPoint(x, y))

    def add_polygon(self):
        x = int(self.polygon_x.value())
        y = int(self.polygon_y.value())

        wind.input_polygon = True

        add_point(QPoint(x, y))

        wind.input_polygon = False

# Добавить точку


def add_point(point):
    global wind, ctrl, now, end_polygon_, end_cutter_
    x = point.x()
    y = point.y()

    arr = None
    table = None
    pen = None

    if wind.input_polygon:
        arr = wind.polygon
        table = wind.table_polygon
        pen = wind.pen_polygon
        if end_polygon_:
            return
    else:
        arr = wind.cutter
        table = wind.table_cutter
        pen = wind.pen_cutter
        if end_cutter_:
            return

    if (len(arr)) == 0:
        arr.append(QPoint(int(point.x()), int(point.y())))

        table.insertRow(table.rowCount())
        i = table.rowCount() - 1
        item_b = QTableWidgetItem(str(point.x()))
        item_e = QTableWidgetItem(str(point.y()))
        table.setItem(i, 0, item_b)
        table.setItem(i, 1, item_e)

    else:
        table.insertRow(table.rowCount())
        i = table.rowCount() - 1
        item_b = QTableWidgetItem(str(point.x()))
        item_e = QTableWidgetItem(str(point.y()))

        table.setItem(i, 0, item_b)
        table.setItem(i, 1, item_e)

        x = point.x()
        y = point.y()

        i = len(arr)
        if ctrl:
            if abs(point.x() - arr[i - 1].x()) < abs(point.y() - arr[i - 1].y()):
                x = arr[i - 1].x()
            elif abs(point.y() - arr[i - 1].y()) < abs(point.x() - arr[i - 1].x()):
                y = arr[i - 1].y()
        wind.scene.addLine(arr[i - 1].x(), arr[i - 1].y(),
                           x, y, pen)
        arr.append(QPoint(int(x), int(y)))


def end_cutter():
    global wind, ctrl, now, end_cutter_

    if not wind.input_cutter:
        return

    end_cutter_ == True
    if (len(wind.cutter)) == 0:
        QMessageBox.warning(wind, "Внимание!",
                            "Чтобы замкнуть, введите отсекатель!")
    elif (len(wind.cutter)) < 3:
        QMessageBox.warning(
            wind, "Внимание!", "Чтобы замкнуть, отсекатель должен иметь не менее 3-х вершин")
    else:
        wind.scene.addLine(wind.cutter[-1].x(), wind.cutter[-1].y(),
                           wind.cutter[0].x(), wind.cutter[0].y(),
                           wind.pen_cutter)


def end_polygon():
    global wind, ctrl, now, end_polygon_

    if wind.draw_cutter.isChecked():
        return

    end_polygon_ == True

    if (len(wind.polygon)) == 0:
        QMessageBox.warning(wind, "Внимание!",
                            "Чтобы замкнуть, введите многоугольник!")
    elif (len(wind.polygon)) < 3:
        QMessageBox.warning(
            wind, "Внимание!", "Чтобы замкнуть, многоугольник должен иметь не менее 3-х вершин")
    else:
        wind.scene.addLine(wind.polygon[-1].x(), wind.polygon[-1].y(),
                           wind.polygon[0].x(), wind.polygon[0].y(),
                           wind.pen_polygon)

###############################################################


def get_d_k_b(ax, ay, cx, cy):
    # Коэффициенты прямой АС
    # Если точки A и С лежат на одной вертикальной прямой
    if abs((cx - ax) - 0) <= 1e-6:
        k = 1
        b = -cx
        d = 0
    else:
        k = (cy - ay) / (cx - ax)
        b = cy - (k * cx)
        d = 1

    return d, k, b


def cross_lines(ax, ay, bx, by, cx, cy, dx, dy):
    d_ab, k_ab, b_ab = get_d_k_b(ax, ay, bx, by)
    d_cd, k_cd, b_cd = get_d_k_b(cx, cy, dx, dy)

    if abs(k_ab - k_cd) < 1e-6:
        return False
    x = (b_cd - b_ab) / (k_ab - k_cd)
    if d_cd == 0:
        y = (k_ab * x + b_ab)
    elif d_ab == 0:
        y = (k_cd * x + b_cd)
    else:
        y = (k_ab * x + b_ab)

    b1, b2 = ax, bx
    ax, bx = max(b1, b2), min(b1, b2)
    b1, b2 = ay, by
    ay, by = max(b1, b2), min(b1, b2)

    if (abs(bx - x) < 1e-6) or (abs(ax - x) < 1e-6) or (abs(by - y) < 1e-6) or (abs(ay - y) < 1e-6):
        return False

    if (bx < x and x < ax) and (by < y and y < ay):
        return True

    return False


def check_cross(arr):
    n = len(arr)
    f = False
    for i in range(n - 1):
        for j in range(i + 1, n, 1):
            if j == n - 1:
                f = cross_lines(arr[i].x(), arr[i].y(), arr[i + 1].x(), arr[i + 1].y(),
                                arr[j].x(), arr[j].y(), arr[0].x(), arr[0].y())
                if f:
                    return True
            else:
                f = cross_lines(arr[i].x(), arr[i].y(), arr[i + 1].x(), arr[i + 1].y(),
                                arr[j].x(), arr[j].y(), arr[j + 1].x(), arr[j + 1].y())
                if f:
                    return True

    return False


def add_polygon(points, pen):
    p = QPolygonF()
    for i in points:
        new_p = QPoint(i.x(), i.y())
        p.append(new_p)

    p_brush = QBrush(wind.color_back)
    wind.scene.addPolygon(p, QPen(pen.color()), p_brush)
##############################################################


def scalar_mult(a, b):
    return a[0] * b[0] + a[1] * b[1]


def vector_mult(a, b):
    # Ax * By - Ay * Bx --- это будет координата Z, которая нам нужна
    return a[0] * b[1] - a[1] * b[0]


def is_convex(arr):
    if len(arr) < 3:
        return False

    a = [arr[0].x() - arr[-1].x(), arr[0].y() - arr[-1].y()]
    b = [arr[-1].x() - arr[-2].x(), arr[-1].y() - arr[-2].y()]
    prev = sign(vector_mult(a, b))
    for i in range(1, len(arr) - 2):
        a = [arr[i].x() - arr[i - 1].x(), arr[i].y() - arr[i - 1].y()]
        b = [arr[i - 1].x() - arr[i - 2].x(), arr[i - 1].y() - arr[i - 2].y()]
        cur = sign(vector_mult(a, b))
        if prev != cur:
            return False
        prev = cur

    if (check_cross(arr)):
        return False

    return True


def normal(a: QPoint, b: QPoint, pos: QPoint):
    fvec = [b.x() - a.x(), b.y() - a.y()]
    posvec = [pos.x() - b.x(), pos.y() - b.y()]

    if fvec[1]:
        fpoint = -fvec[0] / fvec[1]
        normvec = [1, fpoint]
    else:
        normvec = [0, 1]

    if scalar_mult(posvec, normvec) < 0:
        normvec[0] = -normvec[0]
        normvec[1] = -normvec[1]

    return normvec


def lerp(a, b, x):
    return a + (b - a) * x


def sazerland_hodjmen_alg():
    cutter = wind.cutter
    polygon = wind.polygon

    # Проверка отсекателя на выпуклость.
    if not is_convex(cutter):
        QMessageBox.warning(wind, "Внимание!", "Отсекатель невыпусклый!!!")
        return

    # Проверка на замкнутость полигона
    if not end_polygon_:
        end_polygon()

    add_polygon(cutter, wind.pen_cutter)

    count_sides = len(cutter)

    # Цикл по сторонам отсекателя
    for i in range(-2, count_sides  - 2):
        # Вычисление вектора внутренней нормали к очередной
        # i-ой стороне отсекателя - N_вi
        norm = normal(cutter[i], cutter[i + 1], cutter[i + 2])
        # полигон, отсеченный текущей стороной
        cutted_polygon = []

        # цикл по сторонам полигона
        for j in range(-1, len(polygon) - 1):
            p1, p2 = polygon[j], polygon[j + 1]
            # Вычисление вектора W_i=P_1-f_i (f_i берем за вершины стороны)
            w1 = [p1.x() - cutter[i].x(), p1.y() - cutter[i].y()]
            w2 = [p2.x() - cutter[i].x(), p2.y() - cutter[i].y()]

            w1_scal = scalar_mult(w1, norm)
            w2_scal = scalar_mult(w2, norm)

            if w1_scal < 0 and w2_scal < 0:
                #  отрезок вне видимой области
                continue
            elif w1_scal > 0 and w2_scal > 0:
                # отрезок полностью в видимой области
                # p1 была занесена в результат на предыдущем шаге
                cutted_polygon.append(p2)
                continue

            # отрезок пересекает сторону отсекателя

            # Вычисление директрисы отрезка:
            # D = P_2-P_1
            d = [p2.x() - p1.x(), p2.y() - p1.y()]

            d_scal = scalar_mult(d, norm)
            if d_scal == 0:
                if w2_scal < 0:
                    cutted_polygon.append(p2)
                continue

            # Находим коэф пересечения.
            t = -w1_scal / d_scal
            # Точка пересечения
            pt = QPoint(round(lerp(p1.x(), p2.x(), t)),
                        round(lerp(p1.y(), p2.y(), t)))

            if w1_scal < 0:
                # отрезок направлен в сторону внутренней области отсекателя
                cutted_polygon.append(pt)
                cutted_polygon.append(p2)
            else:
                # отрезок направлен от внутренней области отсекателя
                # p1 была занесена в результат на предыдущем шаге
                cutted_polygon.append(pt)

        polygon = cutted_polygon

    add_polygon(polygon, wind.pen_res)


def main():
    global wind
    app = QtWidgets.QApplication(sys.argv)
    wind = MainWindow()
    wind.show()
    app.exec_()


if __name__ == "__main__":
    main()
