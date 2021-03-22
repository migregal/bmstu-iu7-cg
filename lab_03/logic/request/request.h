//
// Created by gregory on 15.03.2021.
//

#ifndef LAB_03_REQUEST_H
#define LAB_03_REQUEST_H

#define ALGS_TITLES_LEN 6

#include <datatypes.h>
#include <drawer_mediator.h>

enum { DRAW_LINE, DRAW_LINE_STEP_COUNT,  DRAW_BUNCH, CLEAR_SCREEN };

enum algs { DDA, BRES_INT, BRES_FLOAT, BRES_ANIT, WU, STD };

struct args {
  int command;
  int method = DDA;
  union {
    line_t line;
    bunch_t bunch;
  };
};

class request {
  const args &arg;
  drawer_mediator &drawer;

protected:
  int draw_line(const color_t &color, bool display = true, bool steps = false);

  void draw_brunch(const color_t &color, bool display = true);

public:
  request(drawer_mediator &mediator, const args &arg);

  int execute(const color_t &color, bool display = true);
};

#endif // LAB_03_REQUEST_H
