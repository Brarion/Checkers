#include "SDL2/i686-w64-mingw32/include/SDL2/SDL.h"
#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_image.h"
#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_mixer.h"
#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_ttf.h"

#include <windows.h>
#include <string>
#include <ctime>
#include <iostream>

#include "server.h"
#include "client.h"


#define N 8

#define MAX_DATA_LEN 512

using namespace std;

typedef unsigned short us;

enum mode
{
  MENU,
  CHOICE,
  GAME
};

enum cell
{
  BLACK_FREE,
  BLACK_USUAL,
  BLACK_QUEEN,
  WHITE_FREE,
  WHITE_USUAL,
  WHITE_QUEEN,
  CAN_DO_STEP
};

enum Player
{
  NONE,
  SERVER,
  CLIENT
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
    if ((event->button.x >= x.first && event->button.x <= x.second) &&
        (event->button.y >= y.first && event->button.y <= y.second))
      return true;

    return false;
  }
};

class Game
{
private:
  /*
    Data:
      1. Textures info
      2. Boards info
  */

  us randomSide;
  
  TTF_Font *font;

  Screen screen;

  SDL_DisplayMode dMode;
  SDL_Window *window;
  SDL_Renderer *ren;

  bool playing;

  mode mode;
  Player player;

  SDL_Rect backgroundRect;
  SDL_Texture *backgroundMenuTexture;

  Button playButton;
  Button exitButton;

  SDL_Texture *backgroundChoiceTexture;
  Button createGameButton;
  Button joinGameButton;

  SDL_Texture *backgroundGameTexture;

  cell board[N][N];
  cell tmpBoard[N][N];

  SDL_Rect boardRect[N][N];
  SDL_Texture *cellTexture[N];
  Button cells[N][N];

  Server server;
  Client client;

  bool start = false;
  bool side;
  bool myStep;
  us steps;

  short x, y;
  pair<us, us> currentFigure;

  // move and board
  //pair<bool, cell **> data;

public:
  // SDL_Init
  Game();

  void loadTextures();

  void showMenu();

  // Player can create the game or join a created game 
  void showChoice();

  // Game itself
  void showGame();

  void findWay(us i, us j);

  void sendData();

  void recvData();

  void play();

  // SDL_Quit
  ~Game();
};