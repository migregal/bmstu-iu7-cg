import sys
from numpy import sign


from time import time
from copy import deepcopy
from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5 import QtWidgets, uic
from PyQt5.QtWidgets import QTableWidgetItem, QMessageBox, QColorDialog
from PyQt5.QtGui import QBrush, QColorConstants, QPen, QColor, QImage, QPolygonF
from PyQt5.QtCore import Qt, QPointF, QLine


now = None
end_cutter_ = False
ctrl = False
wind = None

class Scene(QtWidgets.QGraphicsScene):
    def keyPressEvent(self, event):
        global ctrl
        if event.key() == Qt.Key_Control:
            ctrl = True
        else:
            ctrl = False

    # добавить точку по щелчку мыши
    def mousePressEvent(self, QMouseEvent):
        if (QMouseEvent.button() == Qt.LeftButton) and (end_cutter_ == False):
            add_point(QMouseEvent.scenePos())

        if (QMouseEvent.button() == Qt.RightButton):
            end_cutter()



class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        uic.loadUi("win.ui", self)

        self.graphicsView.scale(1, 1)
        self.scene = Scene(0, 0, 1360, 768)
        self.scene.win = self
        self.graphicsView.setScene(self.scene)
        self.image = QImage(1360, 768, QImage.Format_ARGB32_Premultiplied)
        self.image.fill(Qt.white)

        self.pen_cutter = QtGui.QPen(QColorConstants.Blue)
        self.pen_cutter.setWidth(0)
        self.pen_line = QtGui.QPen(QColorConstants.Green)
        self.pen_line.setWidth(0)
        self.pen_res = QtGui.QPen(QColorConstants.Red)
        self.pen_res.setWidth(0)
        self.input_cutter = False
        self.input_lines = True
        self.lines: list[QLine] = []
        self.cutter: list[QPointF] = []
        self.clip = None
        self.point_now = None
        self.color_back = QtCore.Qt.white

        self.cutter_color.clicked.connect(lambda: self.__get_cutter_color())
        self.cut_line_color.clicked.connect(lambda: self.__get_cut_line_color())

        self.close_cutter.clicked.connect(lambda: end_cutter())

        self.line_color.clicked.connect(lambda: self.__get_line_color())

        self.draw_line.clicked.connect(self.cheng)
        self.draw_cutter.clicked.connect(self.cheng)

        self.clear_screen.clicked.connect(self.__clear_screen)
        self.cut_lines.clicked.connect(cyrus_beck_alg)

        self.add_line.clicked.connect(self.add_line1)
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

    def __get_line_color(self):
        color = QColorDialog.getColor()

        if not QColor.isValid(color):
            return

        self.pen_line.setColor(color)
        hexcolor = color.name()
        self.line_color.setStyleSheet('QPushButton{background-color:' +
                                          hexcolor + '}')

    def cheng(self):
        global now, now_buf
        if self.draw_line.isChecked():
            now_buf = now
            now = None
            self.input_lines = True
            self.input_cutter = False
        elif self.draw_cutter.isChecked():
            self.input_lines = False
            self.input_cutter = True

    def __clear_screen(self):
        global now
        self.scene.clear()
        self.lines = []
        self.cutter = []
        now = None
        self.image.fill(Qt.white)
        r = self.table_line.rowCount()
        for i in range(r, -1, -1):
            self.table_line.removeRow(i)
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


    def add_line1(self):
        try:
            x_start = int(self.line_x_1.value())
            y_start = int(self.line_y_1.value())

            x_end = int(self.line_x_2.value())
            y_end = int(self.line_y_2.value())
        except Exception:
            QMessageBox.warning(self, "Внимание!", "Неверно введены координаты!")
            return

        wind.lines.append(QLine(x_start, y_start, x_end, y_end))

        add_row(wind, 1)
        i = wind.table_line.rowCount() - 1
        item_b = QTableWidgetItem("{0:d}, {1:d}".format(int(x_start), int(y_start)))
        item_e = QTableWidgetItem("{0:d}, {1:d}".format(int(x_end), int(y_end)))
        wind.table_line.setItem(i, 0, item_b)
        wind.table_line.setItem(i, 1, item_e)
        wind.scene.addLine(x_start, y_start,
                            x_end, y_end, wind.pen_line)
        wind.point_now = None

    def add_cutter(self):
        try:
            x = int(self.lineEdit_x.text())
            y = int(self.lineEdit_y.text())

        except Exception:
            QMessageBox.warning(self, "Внимание!", "Неверно введены координаты!")
            return

        add_cutter_vertice(x, y)

# Добавить точку
def add_cutter_vertice(x, y):
    global wind, ctrl, now

    if (len(wind.cutter)) == 0:
        wind.cutter.append(QPointF(x, y))

        add_row(wind, 2)
        i = wind.table_cutter.rowCount() - 1
        item_b = QTableWidgetItem("{0:d}".format(int(x)))
        item_e = QTableWidgetItem("{0:d}".format(int(y)))
        wind.table_cutter.setItem(i, 0, item_b)
        wind.table_cutter.setItem(i, 1, item_e)

    else:
        add_row(wind, 2)
        i = wind.table_cutter.rowCount() - 1
        item_b = QTableWidgetItem("{0:d}".format(int(x)))
        item_e = QTableWidgetItem("{0:d}".format(int(y)))
        wind.table_cutter.setItem(i, 0, item_b)
        wind.table_cutter.setItem(i, 1, item_e)

        i = len(wind.cutter)
        if ctrl:
            if abs(x - wind.cutter[i - 1].x()) < abs(y - wind.cutter[i - 1].y()):
                x = wind.cutter[i - 1].x()
            elif abs(y - wind.cutter[i - 1].y()) < abs(x - wind.cutter[i - 1].x()):
                y = wind.cutter[i - 1].y()
            ctrl = False
        wind.scene.addLine(wind.cutter[i - 1].x(), wind.cutter[i - 1].y(),
                            x, y, wind.pen_cutter)
        wind.cutter.append(QPointF(x, y))


# Добавить строку с координатами с таблицу
def add_row(win, f):
    if f == 1:
        win.table_line.insertRow(win.table_line.rowCount())
    if f == 2:
        win.table_cutter.insertRow(win.table_cutter.rowCount())


# Добавить точку
def add_point(point):
    global wind, ctrl, now
    x = point.x()
    y = point.y()

    if wind.input_lines:
        if wind.point_now is None:
            wind.point_now = point
        else:
            if ctrl:
                if abs(point.x() - wind.point_now.x()) < abs(point.y() - wind.point_now.y()):
                    x = wind.point_now.x()
                elif abs(point.y() - wind.point_now.y()) < abs(point.x() - wind.point_now.x()):
                    y = wind.point_now.y()
                ctrl = False

            wind.lines.append(QLine(int(wind.point_now.x()), int(wind.point_now.y()),
                               int(x), int(y)))

            add_row(wind, 1)
            i = wind.table_line.rowCount() - 1
            item_b = QTableWidgetItem("{0:d}, {1:d}".format(int(wind.point_now.x()), int(wind.point_now.y())))
            item_e = QTableWidgetItem("{0:d}, {1:d}".format(int(point.x()), int(point.y())))
            wind.table_line.setItem(i, 0, item_b)
            wind.table_line.setItem(i, 1, item_e)

            wind.scene.addLine(wind.point_now.x(), wind.point_now.y(),
                               x, y, wind.pen_line)
            wind.point_now = None

    if wind.input_cutter:
        if (len(wind.cutter)) == 0:
            wind.cutter.append(QPointF(int(point.x()), int(point.y())))

            add_row(wind, 2)
            i = wind.table_cutter.rowCount() - 1
            item_b = QTableWidgetItem(str(int(point.x())))
            item_e = QTableWidgetItem(str(int(point.y())))
            wind.table_cutter.setItem(i, 0, item_b)
            wind.table_cutter.setItem(i, 1, item_e)

        else:
            add_row(wind, 2)
            i = wind.table_cutter.rowCount() - 1
            item_b = QTableWidgetItem(str(int(point.x())))
            item_e = QTableWidgetItem(str(int(point.y())))

            wind.table_cutter.setItem(i, 0, item_b)
            wind.table_cutter.setItem(i, 1, item_e)

            x = point.x()
            y = point.y()

            i = len(wind.cutter)
            if ctrl:
                if abs(point.x() - wind.cutter[i - 1].x()) < abs(point.y() - wind.cutter[i - 1].y()):
                    x = wind.cutter[i - 1].y()
                elif abs(point.y() - wind.cutter[i - 1].y()) < abs(point.x() - wind.cutter[i - 1].x()):
                    y = wind.cutter[i - 1].y()
                ctrl = False
            wind.scene.addLine(wind.cutter[i - 1].x(), wind.cutter[i - 1].y(),
                               x, y, wind.pen_cutter)
            wind.cutter.append(QPointF(int(x), int(y)))


def end_cutter():
    global wind, ctrl, now, end_cutter_

    end_cutter_ == True
    if wind.input_cutter:
        if (len(wind.cutter)) == 0:
            QMessageBox.warning(wind, "Внимание!", "Чтобы замкнуть, введите отсекатель!")
        elif (len(wind.cutter)) < 3:
            QMessageBox.warning(wind, "Внимание!", "Чтобы замкнуть, отсекатель должен иметь не менее 3-х вершин")
        else:
            wind.scene.addLine(wind.cutter[-1].x(), wind.cutter[-1].y(),
                               wind.cutter[0].x(), wind.cutter[0].y(),
                               wind.pen_cutter)


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

def add_polygon():
    p = QPolygonF()
    for i in wind.cutter:
        new_p = QPointF(i.x(), i.y())
        p.append(new_p)

    pen = QPen(wind.pen_cutter.color())
    p_brush = QBrush(wind.color_back)
    wind.scene.addPolygon(p, pen, p_brush)
##############################################################



def scalar_mult(a: QPointF, b: QPointF):
    return a.x() * b.x() + a.y() * b.y()


def vector_mult(a: QPointF, b: QPointF):
    return a.x() * b.y() - a.y() * b.x()


def is_convex(arr):
    if len(arr) < 3:
        return False

    a = QPointF(arr[0].x() - arr[-1].x(), arr[0].y() - arr[-1].y())
    b = QPointF(arr[-1].x() - arr[-2].x(), arr[-1].y() - arr[-2].y())
    prev = sign(vector_mult(a, b))
    for i in range(1, len(arr) - 2):
        a = QPointF(arr[i].x() - arr[i - 1].x(), arr[i].y() - arr[i - 1].y())
        b = QPointF(arr[i - 1].x() - arr[i - 2].x(), arr[i - 1].y() - arr[i - 2].y())
        cur = sign(vector_mult(a, b))
        if prev != cur:
            return False
        prev = cur

    if (check_cross(arr)):
        return False

    return True

def normal(a: QPointF, b: QPointF, pos: QPointF):
    fvec = QPointF(b.x() - a.x(), b.y() - a.y())
    posvec = QPointF(pos.x() - b.x(), pos.y() - b.y())

    if fvec.y():
        fpoint = -fvec.x() / fvec.y()
        normvec = QPointF(1, fpoint)
    else:
        normvec = QPointF(0, 1)

    if scalar_mult(posvec, normvec) < 0:
        normvec.setX(-normvec.x())
        normvec.setY(-normvec.y())

    return normvec



def cut_one(line: QLine, count):
    # Вычисление директрисы заданного отрезка:
    # D = P_2-P_1
    d = QPointF(line.x2() - line.x1(), line.y2() - line.y1())

    # Инициализация пределов значений параметра t при условии,
    # что отрезок полностью видим:
    # t_н=0,t_к=1
    top = 0
    bottom = 1

    # Начало цикла по всем сторонам отсекателя.
    # Для каждой i-ой стороны отсекателя выполнить следующие действия:
    for i in range(-2, count - 2):
        # Вычисление вектора внутренней нормали к очередной
        # i-ой стороне отсекателя - N_вi
        norm = normal(wind.cutter[i], wind.cutter[i + 1], wind.cutter[i + 2])

        # Вычисление вектора W_i=P_1-f_i (f_i берем за вершины стороны)
        w = QPointF(line.x1() - wind.cutter[i].x(), line.y1() - wind.cutter[i].y())

        # Вычисление скалярного произведения векторов:
        # W_iскал=W_i N_вi
        # D_скал=DN_вi
        d_scal = scalar_mult(d, norm)
        w_scal = scalar_mult(w, norm)

        # Если D_скал=0, Если W_скi>0, то отрезок
        # (точка) видим(-а) относительно текущей стороны отсекателя
        if d_scal == 0:
            if w_scal < 0:
                return []
            else:
                continue

        # Вычисление параметра t:
        # t=-W_iскал/D_скал
        t = -w_scal / d_scal

        if d_scal > 0:
            if t <= 1:
                top = max(top, t)
            else:
                return
        elif d_scal < 0:
            if t >= 0:
                bottom = min(bottom, t)
            else:
                return


        # Проверка фактической видимости отсечённого отрезка. Если t_н > t_в, то выход
        if top > bottom:
            break

    # Проверка фактической видимости отсечённого отрезка.
    #  Если t_н≤t_в, то изобразить отрезок в
    #  интервале от P(t_н ) до P(t_в ).
    if top <= bottom:
        return QLine(round(line.x1() + d.x() * top), round(line.y1() + d.y() * top),
                round(line.x1() + d.x() * bottom), round(line.y1() + d.y() * bottom))

    return []

# 1 - Ввод исходных данных: точки отрезка P_1 (P_(1.x),P_(1.y) )  и P_2 (P_(2.x),P_(2.y) )
# 2 - Ввод числа сторон m выпуклого многоугольника и координат его вершин (массив C)
def cyrus_beck_alg():
    cutter = wind.cutter

    # Проверка отсекателя на выпуклость.
    if not is_convex(cutter):
        QMessageBox.warning(wind, "Внимание!", "Отсекатель невыпусклый!!!")
        return

    add_polygon()
    lines = wind.lines

    count_sides = len(cutter)

    # Начало цикла по всем сторонам отсекателя.
    drawarr = []
    for line in lines:
        cutt = cut_one(line, count_sides)
        if cutt:
            drawarr.append(cutt)

    draw_lines(drawarr)
    return

def draw_lines(arr: list[QLine]):
    global wind
    for l in arr:
        wind.scene.addLine(l.x2(), l.y2(), l.x1(), l.y1(), wind.pen_res)



def main():
    global wind
    app = QtWidgets.QApplication(sys.argv)
    wind = MainWindow()
    wind.show()
    app.exec_()


if __name__ == "__main__":
    main()
