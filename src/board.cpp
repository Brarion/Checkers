#include "board.h"

Board::Board()
{
  whiteCount = 12;
  blackCount = 12;

  srand(time(NULL));
  for (uc i = 0; i < N; i++)
    for (uc j = 0; j < N; j++)
      board[i][j] = EMPTY;

  uc random = (uc)rand() % 2;
  for (uc i = 1; i < N - 1; i++)
    for (uc j = 1; j < N - 1; j++)
      if (i < 4)
        board[i][j] = ((i + j) % 2) ? ((random) ? BLACK_USUAL : WHITE_USUAL) : WHITE_FREE;
      else if (i > 5)
        board[i][j] = ((i + j) % 2) ? ((random) ? WHITE_USUAL : BLACK_USUAL) : WHITE_FREE;
      else
        board[i][j] = ((i + j) % 2) ? BLACK_FREE : WHITE_FREE;
}

Cell Board::getCell(us x, us y)
{
  return board[x + 1][y + 1];
}

Cell *Board::getBoard()
{
  static Cell tmpBoard[N * N];
  for (uc i = 0; i < N; i++)
    for (uc j = 0; j < N; j++)
      tmpBoard[i * N + j] = board[i][j];

  return tmpBoard;
}

void Board::setBoard(void *data)
{
  Cell *tmp = (Cell *)data;
  for (uc i = 0; i < N; i++)
    for (uc j = 0; j < N; j++)
      board[(N - 1) - i][(N - 1) - j] = tmp[i * N + j];
}

void Board::findWay(us x, us y, uc side, bool isQueen, bool shouldBeat)
{
  x++;
  y++;

  if (side == 0) // Белые ходят
  {
    for (uc i = 1; i < N - 1; i++)
      for (uc j = 1; j < N - 1; j++)
        if (board[i][j] == WHITE_USUAL)
        {
          if (((board[i - 1][j - 1] == BLACK_USUAL || board[i - 1][j - 1] == BLACK_QUEEN) && board[i - 2][j - 2] == BLACK_FREE) ||
              ((board[i - 1][j + 1] == BLACK_USUAL || board[i - 1][j + 1] == BLACK_QUEEN) && board[i - 2][j + 2] == BLACK_FREE) ||
              ((board[i + 1][j - 1] == BLACK_USUAL || board[i + 1][j - 1] == BLACK_QUEEN) && board[i + 2][j - 2] == BLACK_FREE) ||
              ((board[i + 1][j + 1] == BLACK_USUAL || board[i + 1][j + 1] == BLACK_QUEEN) && board[i + 2][j + 2] == BLACK_FREE))
            shouldBeat = true;
        }
        else if (board[i][j] == WHITE_QUEEN)
        {
          bool flag = true;
          for (uc q = i - 1, p = j - 1; q >= 1 && p >= 1 && flag; q--, p--)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q - 1][p - 1] == BLACK_USUAL || board[q - 1][p - 1] == BLACK_QUEEN))
              break;
          for (uc q = i - 1, p = j + 1; q >= 1 && p < N - 1 && flag; q--, p++)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q - 1][p + 1] == BLACK_USUAL || board[q - 1][p + 1] == BLACK_QUEEN))
              break;
          for (uc q = i + 1, p = j - 1; q < N - 1 && p >= 1 && flag; q++, p--)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q + 1][p - 1] == BLACK_USUAL || board[q + 1][p - 1] == BLACK_QUEEN))
              break;
          for (uc q = i + 1, p = j + 1; q < N - 1 && p < N - 1 && flag; q++, p++)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q + 1][p + 1] == BLACK_USUAL || board[q + 1][p + 1] == BLACK_QUEEN))
              break;
        }

    if (shouldBeat)
    {
      if (board[x][y] == WHITE_USUAL)
      {
        if ((board[x - 1][y - 1] == BLACK_USUAL || board[x - 1][y - 1] == BLACK_QUEEN) &&
            board[x - 2][y - 2] == BLACK_FREE)
        {
          board[x - 2][y - 2] = INTERMEDIATE_STEP;
          // this->findWay((x - 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x - 1][y + 1] == BLACK_USUAL || board[x - 1][y + 1] == BLACK_QUEEN) &&
            board[x - 2][y + 2] == BLACK_FREE)
        {
          board[x - 2][y + 2] = INTERMEDIATE_STEP;
          // this->findWay((x - 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y - 1] == BLACK_USUAL || board[x + 1][y - 1] == BLACK_QUEEN) &&
            board[x + 2][y - 2] == BLACK_FREE)
        {
          board[x + 2][y - 2] = INTERMEDIATE_STEP;
          // this->findWay((x + 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y + 1] == BLACK_USUAL || board[x + 1][y + 1] == BLACK_QUEEN) &&
            board[x + 2][y + 2] == BLACK_FREE)
        {
          board[x + 2][y + 2] = INTERMEDIATE_STEP;
          // this->findWay((x + 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }
      }
      else if (board[x][y] == WHITE_QUEEN)
      {
        bool flag = false;
        for (uc q = x - 1, p = y - 1; q >= 1 && p >= 1; q--, p--)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p - 1] == BLACK_FREE && !flag)
          {
            flag = true;
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q - 1][p - 1] == BLACK_USUAL || board[q - 1][p - 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;

        flag = false;
        for (uc q = x - 1, p = y + 1; q >= 1 && p < N - 1; q--, p++)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p + 1] == BLACK_FREE && !flag)
          {
            flag = true;
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q - 1][p + 1] == BLACK_USUAL || board[q - 1][p + 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;

        flag = false;
        for (uc q = x + 1, p = y - 1; q < N - 1 && p >= 1; q++, p--)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p - 1] == BLACK_FREE && !flag)
          {
            flag = true;
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q + 1][p - 1] == BLACK_USUAL || board[q + 1][p - 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;

        flag = false;
        for (uc q = x + 1, p = y + 1; q < N - 1 && p < N - 1; q++, p++)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p + 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q + 1][p + 1] == BLACK_USUAL || board[q + 1][p + 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;
      }
      else if (board[x][y] == INTERMEDIATE_STEP && !isQueen)
      {
        bool flag = false;
        if ((board[x - 1][y - 1] == BLACK_USUAL || board[x - 1][y - 1] == BLACK_QUEEN) &&
            board[x - 2][y - 2] == BLACK_FREE)
        {
          board[x - 2][y - 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x - 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x - 1][y + 1] == BLACK_USUAL || board[x - 1][y + 1] == BLACK_QUEEN) &&
            board[x - 2][y + 2] == BLACK_FREE)
        {
          board[x - 2][y + 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x - 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y - 1] == BLACK_USUAL || board[x + 1][y - 1] == BLACK_QUEEN) &&
            board[x + 2][y - 2] == BLACK_FREE)
        {
          board[x + 2][y - 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x + 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y + 1] == BLACK_USUAL || board[x + 1][y + 1] == BLACK_QUEEN) &&
            board[x + 2][y + 2] == BLACK_FREE)
        {
          board[x + 2][y + 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x + 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }

        if (!flag)
          board[x][y] = FINAL_STEP;
      }
      else if (board[x][y] == INTERMEDIATE_STEP && isQueen)
      {
        bool FLAG = false;
        bool flag = false;
        for (uc q = x - 1, p = y - 1; q >= 1 && p >= 1; q--, p--)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p - 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q - 1][p - 1] == BLACK_USUAL || board[q - 1][p - 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        flag = false;
        for (uc q = x - 1, p = y + 1; q >= 1 && p < N - 1; q--, p++)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p + 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q - 1][p + 1] == BLACK_USUAL || board[q - 1][p + 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        flag = false;
        for (uc q = x + 1, p = y - 1; q < N - 1 && p >= 1; q++, p--)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p - 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q + 1][p - 1] == BLACK_USUAL || board[q + 1][p - 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        flag = false;
        for (uc q = x + 1, p = y + 1; q < N - 1 && p < N - 1; q++, p++)
          if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p + 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                       (board[q + 1][p + 1] == BLACK_USUAL || board[q + 1][p + 1] == BLACK_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        if (!FLAG)
          board[x][y] = FINAL_STEP;
      }
    }
    else // Не надо бить
    {
      if (board[x][y] == WHITE_USUAL) // Белая шашка
      {
        if (board[x - 1][y - 1] == BLACK_FREE)
          board[x - 1][y - 1] = FINAL_STEP;
        if (board[x - 1][y + 1] == BLACK_FREE)
          board[x - 1][y + 1] = FINAL_STEP;
      }
      else if (board[x][y] == WHITE_QUEEN) // Белая королева
      {

        for (uc q = x - 1, p = y - 1; q >= 1 && p >= 1; q--, p--)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;

        for (uc q = x - 1, p = y + 1; q >= 1 && p < N - 1; q--, p++)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;

        for (uc q = x + 1, p = y - 1; q < N - 1 && p >= 1; q++, p--)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;

        for (uc q = x + 1, p = y + 1; q < N - 1 && p < N - 1; q++, p++)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;
      }
    }
  }
  else // Черные ходят
  {
    for (uc i = 1; i < N - 1; i++)
      for (uc j = 1; j < N - 1; j++)
        if (board[i][j] == BLACK_USUAL)
        {
          if (((board[i - 1][j - 1] == WHITE_USUAL || board[i - 1][j - 1] == WHITE_QUEEN) && board[i - 2][j - 2] == BLACK_FREE) ||
              ((board[i - 1][j + 1] == WHITE_USUAL || board[i - 1][j + 1] == WHITE_QUEEN) && board[i - 2][j + 2] == BLACK_FREE) ||
              ((board[i + 1][j - 1] == WHITE_USUAL || board[i + 1][j - 1] == WHITE_QUEEN) && board[i + 2][j - 2] == BLACK_FREE) ||
              ((board[i + 1][j + 1] == WHITE_USUAL || board[i + 1][j + 1] == WHITE_QUEEN) && board[i + 2][j + 2] == BLACK_FREE))
            shouldBeat = true;
        }
        else if (board[i][j] == BLACK_QUEEN)
        {
          bool flag = true;
          for (uc q = i - 1, p = j - 1; q >= 1 && p >= 1 && flag; q--, p--)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q - 1][p - 1] == WHITE_USUAL || board[q - 1][p - 1] == WHITE_QUEEN))
              break;
          for (uc q = i - 1, p = j + 1; q >= 1 && p < N - 1 && flag; q--, p++)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q - 1][p + 1] == WHITE_USUAL || board[q - 1][p + 1] == WHITE_QUEEN))
              break;
          for (uc q = i + 1, p = j - 1; q < N - 1 && p >= 1 && flag; q++, p--)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q + 1][p - 1] == WHITE_USUAL || board[q + 1][p - 1] == WHITE_QUEEN))
              break;
          for (uc q = i + 1, p = j + 1; q < N - 1 && p < N - 1 && flag; q++, p++)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q + 1][p + 1] == WHITE_USUAL || board[q + 1][p + 1] == WHITE_QUEEN))
              break;
        }

    if (shouldBeat)
    {
      if (board[x][y] == BLACK_USUAL)
      {
        if ((board[x - 1][y - 1] == WHITE_USUAL || board[x - 1][y - 1] == WHITE_QUEEN) &&
            board[x - 2][y - 2] == BLACK_FREE)
        {
          board[x - 2][y - 2] = INTERMEDIATE_STEP;
          // this->findWay((x - 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x - 1][y + 1] == WHITE_USUAL || board[x - 1][y + 1] == WHITE_QUEEN) &&
            board[x - 2][y + 2] == BLACK_FREE)
        {
          board[x - 2][y + 2] = INTERMEDIATE_STEP;
          // this->findWay((x - 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y - 1] == WHITE_USUAL || board[x + 1][y - 1] == WHITE_QUEEN) &&
            board[x + 2][y - 2] == BLACK_FREE)
        {
          board[x + 2][y - 2] = INTERMEDIATE_STEP;
          // this->findWay((x + 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y + 1] == WHITE_USUAL || board[x + 1][y + 1] == WHITE_QUEEN) &&
            board[x + 2][y + 2] == BLACK_FREE)
        {
          board[x + 2][y + 2] = INTERMEDIATE_STEP;
          // this->findWay((x + 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }
      }
      else if (board[x][y] == BLACK_QUEEN)
      {
        bool flag = false;
        for (uc q = x - 1, p = y - 1; q >= 1 && p >= 1; q--, p--)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p - 1] == BLACK_FREE && !flag)
          {
            flag = true;
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q - 1][p - 1] == WHITE_USUAL || board[q - 1][p - 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;

        flag = false;
        for (uc q = x - 1, p = y + 1; q >= 1 && p < N - 1; q--, p++)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p + 1] == BLACK_FREE && !flag)
          {
            flag = true;
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q - 1][p + 1] == WHITE_USUAL || board[q - 1][p + 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;

        flag = false;
        for (uc q = x + 1, p = y - 1; q < N - 1 && p >= 1; q++, p--)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p - 1] == BLACK_FREE && !flag)
          {
            flag = true;
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q + 1][p - 1] == WHITE_USUAL || board[q + 1][p - 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;

        flag = false;
        for (uc q = x + 1, p = y + 1; q < N - 1 && p < N - 1; q++, p++)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p + 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q + 1][p + 1] == WHITE_USUAL || board[q + 1][p + 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;
      }
      else if (board[x][y] == INTERMEDIATE_STEP && !isQueen)
      {
        bool flag = false;
        if ((board[x - 1][y - 1] == WHITE_USUAL || board[x - 1][y - 1] == WHITE_QUEEN) &&
            board[x - 2][y - 2] == BLACK_FREE)
        {
          board[x - 2][y - 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x - 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x - 1][y + 1] == WHITE_USUAL || board[x - 1][y + 1] == WHITE_QUEEN) &&
            board[x - 2][y + 2] == BLACK_FREE)
        {
          board[x - 2][y + 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x - 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y - 1] == WHITE_USUAL || board[x + 1][y - 1] == WHITE_QUEEN) &&
            board[x + 2][y - 2] == BLACK_FREE)
        {
          board[x + 2][y - 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x + 2) - 1, (y - 2) - 1, side, false, shouldBeat);
        }
        if ((board[x + 1][y + 1] == WHITE_USUAL || board[x + 1][y + 1] == WHITE_QUEEN) &&
            board[x + 2][y + 2] == BLACK_FREE)
        {
          board[x + 2][y + 2] = INTERMEDIATE_STEP;
          flag = true;
          // this->findWay((x + 2) - 1, (y + 2) - 1, side, false, shouldBeat);
        }

        if (!flag)
          board[x][y] = FINAL_STEP;
      }
      else if (board[x][y] == INTERMEDIATE_STEP && isQueen)
      {
        bool FLAG = false;
        bool flag = false;
        for (uc q = x - 1, p = y - 1; q >= 1 && p >= 1; q--, p--)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p - 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q - 1][p - 1] == WHITE_USUAL || board[q - 1][p - 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        flag = false;
        for (uc q = x - 1, p = y + 1; q >= 1 && p < N - 1; q--, p++)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p + 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q - 1][p + 1] == WHITE_USUAL || board[q - 1][p + 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        flag = false;
        for (uc q = x + 1, p = y - 1; q < N - 1 && p >= 1; q++, p--)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p - 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q + 1][p - 1] == WHITE_USUAL || board[q + 1][p - 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        flag = false;
        for (uc q = x + 1, p = y + 1; q < N - 1 && p < N - 1; q++, p++)
          if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p + 1] == BLACK_FREE && !flag)
            flag = true;
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                       (board[q + 1][p + 1] == WHITE_USUAL || board[q + 1][p + 1] == WHITE_QUEEN) ||
                   (board[q][p] == WHITE_USUAL || board[q][p] == BLACK_QUEEN))
            break;
          else if (flag && board[q][p] == BLACK_FREE)
          {
            board[q][p] = INTERMEDIATE_STEP;
            // this->findWay(q - 1, p - 1, side, true, shouldBeat);
          }
          else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && flag)
            break;

        if (flag)
          FLAG = true;

        if (!FLAG)
          board[x][y] = FINAL_STEP;
      }
    }
    else // Не надо бить
    {

      if (board[x][y] == BLACK_USUAL) // Черная шашка
      {
        if (board[x - 1][y - 1] == BLACK_FREE)
          board[x - 1][y - 1] = FINAL_STEP;
        if (board[x - 1][y + 1] == BLACK_FREE)
          board[x - 1][y + 1] = FINAL_STEP;
      }
      else if (board[x][y] == BLACK_QUEEN) // Черная королева
      {

        for (uc q = x - 1, p = y - 1; q >= 1 && p >= 1; q--, p--)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;

        for (uc q = x - 1, p = y + 1; q >= 1 && p < N - 1; q--, p++)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;

        for (uc q = x + 1, p = y - 1; q < N - 1 && p >= 1; q++, p--)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;

        for (uc q = x + 1, p = y + 1; q < N - 1 && p < N - 1; q++, p++)
          if (board[q][p] == BLACK_FREE)
            board[q][p] = FINAL_STEP;
          else
            break;
      }
    }
  }
}

void Board::clear()
{
  for (uc i = 1; i < N - 1; i++)
    for (uc j = 1; j < N - 1; j++)
      if ((board[i][j] == INTERMEDIATE_STEP) || (board[i][j] == FINAL_STEP))
        board[i][j] = BLACK_FREE;
}

us Board::getCantMove(uc side)
{
  if (whiteCount == 0)
    return 1;
  else if (blackCount == 0)
    return 2;

  if (side == 0)
  {
    bool shouldBeat = false;

    for (uc i = 1; i < N - 1; i++)
      for (uc j = 1; j < N - 1; j++)
        if (board[i][j] == WHITE_USUAL)
        {
          if (((board[i - 1][j - 1] == BLACK_USUAL || board[i - 1][j - 1] == BLACK_QUEEN) && board[i - 2][j - 2] == BLACK_FREE) ||
              ((board[i - 1][j + 1] == BLACK_USUAL || board[i - 1][j + 1] == BLACK_QUEEN) && board[i - 2][j + 2] == BLACK_FREE) ||
              ((board[i + 1][j - 1] == BLACK_USUAL || board[i + 1][j - 1] == BLACK_QUEEN) && board[i + 2][j - 2] == BLACK_FREE) ||
              ((board[i + 1][j + 1] == BLACK_USUAL || board[i + 1][j + 1] == BLACK_QUEEN) && board[i + 2][j + 2] == BLACK_FREE))
            shouldBeat = true;
        }
        else if (board[i][j] == WHITE_QUEEN)
        {
          bool flag = true;
          for (uc q = i - 1, p = j - 1; q >= 1 && p >= 1 && flag; q--, p--)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q - 1][p - 1] == BLACK_USUAL || board[q - 1][p - 1] == BLACK_QUEEN))
              break;
          for (uc q = i - 1, p = j + 1; q >= 1 && p < N - 1 && flag; q--, p++)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q - 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q - 1][p + 1] == BLACK_USUAL || board[q - 1][p + 1] == BLACK_QUEEN))
              break;
          for (uc q = i + 1, p = j - 1; q < N - 1 && p >= 1 && flag; q++, p--)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q + 1][p - 1] == BLACK_USUAL || board[q + 1][p - 1] == BLACK_QUEEN))
              break;
          for (uc q = i + 1, p = j + 1; q < N - 1 && p < N - 1 && flag; q++, p++)
            if (flag && (board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) && board[q + 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == BLACK_USUAL || board[q][p] == BLACK_QUEEN) &&
                     (board[q + 1][p + 1] == BLACK_USUAL || board[q + 1][p + 1] == BLACK_QUEEN))
              break;
        }

    // Проверка, есть ли возможность ходить
    cantMove = false;
    for (uc i = 1; i < N - 1; i++)
      for (uc j = 1; j < N - 1; j++)
        if (board[i][j] == WHITE_USUAL || board[i][j] == WHITE_QUEEN)
          if (board[i - 1][j - 1] == BLACK_FREE || board[i - 1][j - 1] == INTERMEDIATE_STEP || board[i - 1][j - 1] == FINAL_STEP ||
              board[i - 1][j + 1] == BLACK_FREE || board[i - 1][j + 1] == INTERMEDIATE_STEP || board[i - 1][j + 1] == FINAL_STEP ||
              board[i + 1][j - 1] == BLACK_FREE || board[i + 1][j - 1] == INTERMEDIATE_STEP || board[i + 1][j - 1] == FINAL_STEP ||
              board[i + 1][j + 1] == BLACK_FREE || board[i + 1][j + 1] == INTERMEDIATE_STEP || board[i + 1][j + 1] == FINAL_STEP)
            cantMove = true;

    if (cantMove == false && shouldBeat == false)
      return 1;
  }
  else
  {
    bool shouldBeat = false;
    for (uc i = 1; i < N - 1; i++)
      for (uc j = 1; j < N - 1; j++)
        if (board[i][j] == BLACK_USUAL)
        {
          if (((board[i - 1][j - 1] == WHITE_USUAL || board[i - 1][j - 1] == WHITE_QUEEN) && board[i - 2][j - 2] == BLACK_FREE) ||
              ((board[i - 1][j + 1] == WHITE_USUAL || board[i - 1][j + 1] == WHITE_QUEEN) && board[i - 2][j + 2] == BLACK_FREE) ||
              ((board[i + 1][j - 1] == WHITE_USUAL || board[i + 1][j - 1] == WHITE_QUEEN) && board[i + 2][j - 2] == BLACK_FREE) ||
              ((board[i + 1][j + 1] == WHITE_USUAL || board[i + 1][j + 1] == WHITE_QUEEN) && board[i + 2][j + 2] == BLACK_FREE))
            shouldBeat = true;
        }
        else if (board[i][j] == BLACK_QUEEN)
        {
          bool flag = true;
          for (uc q = i - 1, p = j - 1; q >= 1 && p >= 1 && flag; q--, p--)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q - 1][p - 1] == WHITE_USUAL || board[q - 1][p - 1] == WHITE_QUEEN))
              break;
          for (uc q = i - 1, p = j + 1; q >= 1 && p < N - 1 && flag; q--, p++)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q - 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q - 1][p + 1] == WHITE_USUAL || board[q - 1][p + 1] == WHITE_QUEEN))
              break;
          for (uc q = i + 1, p = j - 1; q < N - 1 && p >= 1 && flag; q++, p--)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p - 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q + 1][p - 1] == WHITE_USUAL || board[q + 1][p - 1] == WHITE_QUEEN))
              break;
          for (uc q = i + 1, p = j + 1; q < N - 1 && p < N - 1 && flag; q++, p++)
            if (flag && (board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) && board[q + 1][p + 1] == BLACK_FREE)
            {
              shouldBeat = true;
              flag = false;
              break;
            }
            else if ((board[q][p] == WHITE_USUAL || board[q][p] == WHITE_QUEEN) &&
                     (board[q + 1][p + 1] == WHITE_USUAL || board[q + 1][p + 1] == WHITE_QUEEN))
              break;
        }

    // Проверка, есть ли возможность ходить
    cantMove = false;
    for (uc i = 1; i < N - 1; i++)
      for (uc j = 1; j < N - 1; j++)
        if (board[i][j] == BLACK_USUAL || board[i][j] == BLACK_QUEEN)
          if (board[i - 1][j - 1] == BLACK_FREE || board[i - 1][j - 1] == INTERMEDIATE_STEP || board[i - 1][j - 1] == FINAL_STEP ||
              board[i - 1][j + 1] == BLACK_FREE || board[i - 1][j + 1] == INTERMEDIATE_STEP || board[i - 1][j + 1] == FINAL_STEP ||
              board[i + 1][j - 1] == BLACK_FREE || board[i + 1][j - 1] == INTERMEDIATE_STEP || board[i + 1][j - 1] == FINAL_STEP ||
              board[i + 1][j + 1] == BLACK_FREE || board[i + 1][j + 1] == INTERMEDIATE_STEP || board[i + 1][j + 1] == FINAL_STEP)
            cantMove = true;

    if (cantMove == false && shouldBeat == false)
      return 2;
  }

  return 0;
}

uc Board::move(us x0, us y0, us x1, us y1)
{
  if (abs(x1 - x0) == 1)
    this->clear();
  else
    board[x1 + 1][y1 + 1] = BLACK_FREE;

  swap(board[++x0][++y0], board[++x1][++y1]);

  if (x1 == 1 || x1 == N - 2)
  {
    if (board[x1][y1] == WHITE_USUAL)
      board[x1][y1] = WHITE_QUEEN;
    else if (board[x1][y1] == BLACK_USUAL)
      board[x1][y1] = BLACK_QUEEN;
  }

  for (uc i = x0 + (x1 - x0) / abs(x1 - x0), j = y0 + (y1 - y0) / abs(y1 - y0); i != x1, j != y1; i += (x1 - x0) / abs(x1 - x0), j += (y1 - y0) / abs(y1 - y0))
    if (board[i][j] == WHITE_USUAL || board[i][j] == WHITE_QUEEN)
    {
      board[i][j] = BLACK_FREE;
      whiteCount--;
      if (whiteCount == 0)
        return 1;
    }
    else if (board[i][j] == BLACK_USUAL || board[i][j] == BLACK_QUEEN)
    {
      board[i][j] = BLACK_FREE;
      blackCount--;
      if (blackCount == 0)
        return 2;
    }

  return 0;
}

uc Board::size()
{
  return N - 2;
}

Board::~Board()
{
}