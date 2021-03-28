//
// Created by gregory on 15.03.2021.
//

#ifndef LAB_04_REQUEST_H
#define LAB_04_REQUEST_H

#define ALGS_TITLES_LEN 5

#include <vector>

#include <datatypes.h>
#include <drawer_mediator.h>

enum {
  DRAW_CIRCLE,
  DRAW_ELLIPSE,
  DRAW_CIRCLE_BUNCH,
  DRAW_ELLIPSE_BUNCH,
  MEASURE_CIRCLE_TIMES,
  MEASURE_ELLIPSE_TIMES,
  CLEAR_SCREEN
};

enum algs { CANONICAL, PARAMETRIC, BRESENHAM, MID_POINT, STD };

struct args {
  int command = CLEAR_SCREEN;
  int method = CANONICAL;
  union {
    circle_t circle;
    ellipse_t ellipse;

    circle_bunch_t c_bunch;
    ellipse_bunch_t e_bunch;

    measure_circle_bunch_t m_c_bunch;
    measure_ellipse_bunch_t m_e_bunch;
  };

  [[nodiscard]] inline circle_bunch_t get_c_bunch() const { return c_bunch; }

  [[nodiscard]] inline ellipse_bunch_t get_e_bunch() const { return e_bunch; }

  [[nodiscard]] inline const measure_circle_bunch_t &get_m_c_bunch() const {
    return m_c_bunch;
  }

  [[nodiscard]] inline const measure_ellipse_bunch_t &get_m_e_bunch() const {
    return m_e_bunch;
  }
};

class request {
  const args &arg;
  drawer_mediator &drawer;

protected:
  void draw_circle(const color_t &color, bool display);

  void draw_ellipse(const color_t &color, bool display);

  void draw_circle_brunch(const color_t &color, bool display = true);

  void draw_ellipse_brunch(const color_t &color, bool display = true);

  void measure_avg_circle_times(bool display = false);

  void measure_avg_ellipse_times(bool display = false);

public:
  request(drawer_mediator &mediator, const args &arg);

  void execute(const color_t &color, bool display = true);
};

#endif // LAB_04_REQUEST_H
