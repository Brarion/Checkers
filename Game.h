#include "SDL2/i686-w64-mingw32/include/SDL2/SDL.h"
#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_image.h"
#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_mixer.h"

#include <windows.h>
#include <iostream>

using namespace std;

typedef unsigned short us;

enum mode
{
  MENU,
  MODE
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

  Screen screen;

  SDL_DisplayMode dMode;
  SDL_Window *window;
  SDL_Renderer *ren;

  bool playing;

  mode mode;

  SDL_Rect backgroundRect;
  SDL_Texture *backgroundTexture;

  SDL_Rect chessBoardRect;
  SDL_Texture *chessBoardTexture;

  Button playButton;
  Button exitButton;

public:
  // SDL_Init
  Game();

  void loadTextures();

  void showMenu();

  // Game itself
  void showGame();

  void play();

  // SDL_Quit
  ~Game();
};