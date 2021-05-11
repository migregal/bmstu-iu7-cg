from PyQt5.QtCore import QPoint, QLine
from PyQt5.QtWidgets import QMessageBox


class Line:
    def __init__(self, _x1: int = 0, _y1: int = 0, _x2: int = 0, _y2: int = 0,
                 _item=None):
        self.line: QLine = QLine(QPoint(_x1, _y1), QPoint(_x2, _y2))
        self.scene_item = _item


class Rect:
    def __init__(self, _xl: int = 0, _xr: int = 0, _yt: int = 0, _yb: int = 0):
        self.__left = _xl
        self.__right = _xr
        self.__top = _yb
        self.__bottom = _yt

    def top(self):
        return self.__top

    def bottom(self):
        return self.__bottom

    def left(self):
        return self.__left

    def right(self):
        return self.__right


class Cutter:
    def __init__(self, _xl: int = 0, _xr: int = 0, _yt: int = 0, _yb: int = 0,
                 _item=None):
        self.rect: Rect = Rect(_xl, _xr, _yt, _yb)
        self.scene_item = _item


# Выводит окно с предупреждением
def mes(text):
    msg = QMessageBox()
    msg.setIcon(QMessageBox.Warning)

    msg.setWindowTitle("Внимание")
    msg.setText(text)

    msg.setStandardButtons(QMessageBox.Ok)

    msg.exec_()


def add_line(self, x1: int, y1: int, x2: int, y2: int, color):
    self.pen.setColor(color)
    self.lines.append(
        Line(
            x1, y1, x2, y2,
            self.scene.addLine(x1, y1, x2, y2, self.pen)
        )
    )
    self.scene.addLine(x1, y1, x2, y2, self.pen)


def add_cutter(self, _xl, _yt, _xr, _yb, color):
    self.pen.setColor(color)

    if _xl > _xr:
        _xl, _xr = _xr, _xl

    if _yt > _yb:
        _yt, _yb = _yb, _yt

    self.cutter = Cutter(
        _xl, _xr, _yt, _yb,
        self.scene.addRect(_xl, _yt, _xr - _xl, _yb - _yt, self.pen)
    )


def del_cutter(self):
    if self.cutter:
        self.scene.removeItem(self.cutter.scene_item)
    self.cutter = None


def line_on_screen(self, x, y):
    if not self.drawing_cutter:
        if self.ctrl_pressed == 0 or len(self.cur_line) == 0:
            self.cur_line.append((x, y))

        else:
            prev = self.cur_line[0]

            dx = x - prev[0]
            dy = y - prev[1]

            if abs(dy) >= abs(dx):
                self.cur_line.append((prev[0], y))
            else:
                self.cur_line.append((x, prev[1]))

        if len(self.cur_line) == 2:
            c1, c2 = self.cur_line
            add_line(self, c1[0], c1[1], c2[0], c2[1], self.line_color)
            self.cur_line.clear()
            self.scene.removeItem(self.follow_line)


def cutter_on_screen(self, x, y):
    if len(self.cur_cutter) < 2:
        self.cur_cutter.append((x, y))

    if len(self.cur_cutter) == 2:
        c1, c2 = self.cur_cutter
        add_cutter(self, c1[0], c1[1], c2[0], c2[1], self.cutter_color)
        self.cur_cutter.clear()
        self.scene.removeItem(self.follow_cutter)
        self.drawing_cutter = False


def following_line(self, x, y):
    if len(self.cur_line) == 1:
        prev = self.cur_line[0]
        self.pen.setColor(self.line_color)

        if self.follow_line:
            self.scene.removeItem(self.follow_line)

        if self.ctrl_pressed:
            dx = x - prev[0]
            dy = y - prev[1]

            if abs(dy) >= abs(dx):
                cur = (prev[0], y)
            else:
                cur = (x, prev[1])

            self.follow_line = self.scene.addLine(prev[0], prev[1], cur[0],
                                                  cur[1], self.pen)
        else:
            self.follow_line = self.scene.addLine(prev[0], prev[1], x, y,
                                                  self.pen)


def following_cutter(self, x, y):
    if len(self.cur_cutter) == 1:
        x_l, y_u = self.cur_cutter[0]
        x_r, y_d = x, y
        self.pen.setColor(self.cutter_color)

        if self.follow_cutter:
            self.scene.removeItem(self.follow_cutter)

        if x_l > x_r:
            x_l, x_r = x_r, x_l
        if y_u > y_d:
            y_u, y_d = y_d, y_u

        self.follow_cutter = self.scene.addRect(x_l, y_u, x_r - x_l, y_d - y_u,
                                                self.pen)


def draw_line(self, dot1: QPoint, dot2: QPoint, color):
    self.pen.setColor(color)

    self.colored_lines.append(self.scene.addLine(
        dot1.x(), dot1.y(), dot2.x(), dot2.y(), self.pen))
