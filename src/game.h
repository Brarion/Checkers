#include "../SDL2/i686-w64-mingw32/include/SDL2/SDL.h"
#include "../SDL2/i686-w64-mingw32/include/SDL2/SDL_image.h"
#include "../SDL2/i686-w64-mingw32/include/SDL2/SDL_mixer.h"
#include "../SDL2/i686-w64-mingw32/include/SDL2/SDL_ttf.h"

#include <iostream>
#include <vector>

#include "board.h"
#include "network.h"

using namespace std;

#ifndef GAME_H
#define GAME_H

#define N 10

typedef unsigned char uc;
typedef unsigned short us;

enum Side
{
  WHITE,
  BLACK
};

enum Mode
{
  MENU,
  CHOICE,
  GAME,
  WHITE_WIN,
  BLACK_WIN
};

enum Player
{
  NONE,
  HOST,
  GUEST
};

struct Screen
{
  us width;
  us height;

  void set(us _width, us _height)
  {
    width = _width;
    height = _height;
  }
};

struct Button
{
  pair<us, us> x;
  pair<us, us> y;

  void set(us x1,
           us x2,
           us y1,
           us y2)
  {
    x = pair<us, us>(x1, x2);
    y = pair<us, us>(y1, y2);
  }

  bool isPressed(SDL_Event *event)
  {
    return ((event->button.x >= x.first && event->button.x <= x.second) &&
            (event->button.y >= y.first && event->button.y <= y.second));
  }
};

class Game
{
private:
  bool playing;
  Mode mode;
  Player player;
  bool myStep;
  Side side;

  bool isQueen, flag;

  pair<us, us> currentChecker;

  SDL_DisplayMode dMode;
  SDL_Window *window;
  SDL_Renderer *ren;

  Screen screen;

  SDL_Rect backgroundRect;
  SDL_Texture *backgroundMenuTexture;
  SDL_Texture *backgroundChoiceTexture;
  SDL_Texture *backgroundGameTexture;

  SDL_Rect boardRect[N - 2][N - 2];
  SDL_Texture *cellTexture[N - 2];

  Button cells[N - 2][N - 2];
  Button playButton;
  Button exitButton;
  Button createGameButton;
  Button joinGameButton;

  Board board;
  Network network;

  TTF_Font *font;
  SDL_Rect textRect;


public:
  Game();

  void loadTextures();
  void play();

  void show();

  ~Game();
};

#endif // GAME_H