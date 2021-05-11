import copy

from PyQt5.QtCore import QPoint, QLine

from funcs import Rect


def count_S(T):
    return sum(T)


def count_pl(t1, t2):
    p = 0

    for i in range(len(t1)):
        p += (t1[i] * t2[i])

    return p


def count_T(p: QPoint, r: Rect):
    return [
        1 if p.x() < r.left() else 0,
        1 if p.x() > r.right() else 0,
        1 if p.y() < r.bottom() else 0,
        1 if p.y() > r.top() else 0
    ]


def simple_cut(cutter: Rect, line: QLine):
    t1, t2 = count_T(line.p1(), cutter), count_T(line.p2(), cutter)
    s1, s2 = count_S(t1), count_S(t2)

    flag = 1

    r1, r2 = copy.deepcopy(line.p1()), copy.deepcopy(line.p2())

    # отрезок полностью видимый
    if (s1 == 0) and (s2 == 0):
        return step_31(flag, r1, r2)

    pl = count_pl(t1, t2)

    # отрезок лежит по одну из сторон окна (невидимый)- тривиальная невидимость
    if pl != 0:
        flag = -1
        return step_31(flag)

    # отрезок частично видимый
    if s1 == 0:
        r1 = copy.deepcopy(line.p1())
        q = copy.deepcopy(line.p2())
        i = 2
        return step_15(flag, i, q, line, r1, r2, cutter)

    if s2 == 0:
        r1 = copy.deepcopy(line.p2())
        q = copy.deepcopy(line.p1())
        i = 2
        return step_15(flag, i, q, line, r1, r2, cutter)

    # внутри окна нет концов отрезка
    i = 0
    return step_12(flag, i, line, r1, r2, cutter)


def step_31(flag, p1: QPoint = None, p2: QPoint = None):
    if flag == 1:
        return True, p1, p2
    else:
        return False, p1, p2


def step_23(flag: int, i: int, q: QPoint, line: QLine, r1: QPoint, r2: QPoint,
            cutter: Rect, m: float):
    if m == 0:
        return step_12(flag, i, line, r1, r2, cutter)

    if q.y() < cutter.bottom():
        x = (cutter.bottom() - q.y()) / m + q.x()

        if cutter.left() <= x <= cutter.right():
            if i == 1:
                r1.setX(x)
                r1.setY(cutter.bottom())
            else:
                r2.setX(x)
                r2.setY(cutter.bottom())

            # проверка пересения с нижним краем
            return step_12(flag, i, line, r1, r2, cutter)

    if q.y() > cutter.top():
        x = (cutter.top() - q.y()) / m + q.x()

        if cutter.left() <= x <= cutter.right():
            if i == 1:
                r1.setX(x)
                r1.setY(cutter.top())
            else:
                r2.setX(x)
                r2.setY(cutter.top())

            # проверка пересечения с верхним краем
            return step_12(flag, i, line, r1, r2, cutter)

    flag = -1
    return step_31(flag)


def step_15(flag: int, i: int, q: QPoint, line: QLine, r1: QPoint, r2: QPoint,
            cutter: Rect):
    # проверка вертикальности отрезка
    if line.x1() == line.x2():
        return step_23(flag, i, q, line, r1, r2, cutter, 1e30)

    m = (line.y2() - line.y1()) / (line.x2() - line.x1())

    # step 17
    if q.x() < cutter.left():
        y = m * (cutter.left() - q.x()) + q.y()

        # обнаружено корректное пересечение
        if cutter.bottom() <= y <= cutter.top():
            if i == 1:
                r1.setX(cutter.left())
                r1.setY(y)
            else:
                r2.setX(cutter.left())
                r2.setY(y)

            # проверка пересечения с левым краем
            return step_12(flag, i, line, r1, r2, cutter)

    # step 20
    if q.x() > cutter.right():
        y = m * (cutter.right() - q.x()) + q.y()

        # обнаружено корректное пересечение
        if cutter.bottom() <= y <= cutter.top():
            if i == 1:
                r1.setX(cutter.right())
                r1.setY(y)
            else:
                r2.setX(cutter.right())
                r2.setY(y)

            # проверка пересечения с правым крааем
            return step_12(flag, i, line, r1, r2, cutter)

    return step_23(flag, i, q, line, r1, r2, cutter, m)


def step_12(flag: int, i: int, line: QLine, r1: QPoint, r2: QPoint,
            cutter: Rect):
    # внутри окна нет концов отрезков

    i += 1
    if i > 2:
        return step_31(flag, r1, r2)

    q = line.p1() if i == 1 else line.p2()

    return step_15(flag, i, q, line, r1, r2, cutter)
