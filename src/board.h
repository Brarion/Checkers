#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

#ifndef BOARD_H
#define BOARD_H

#define N 10

typedef unsigned char uc;
typedef unsigned short us;

enum Cell
{
  EMPTY,
  BLACK_FREE,
  BLACK_USUAL,
  BLACK_QUEEN,
  WHITE_FREE,
  WHITE_USUAL,
  WHITE_QUEEN,
  INTERMEDIATE_STEP,
  FINAL_STEP
};

class Board
{
private:
  Cell board[N][N];

  uc whiteCount;
  uc blackCount;

  bool cantMove;

public:
  Board();

  Cell getCell(us x, us y);
  Cell *getBoard();
  void setBoard(void *data);
  void findWay(us x, us y, uc side, bool isQueen, bool shouldBeat);
  void clear();
  us getCantMove(uc side);
  uc move(us x0, us y0, us x1, us y1);
  uc size();

  ~Board();
};

#endif // BOARD_H