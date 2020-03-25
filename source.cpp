/*
  TODO:
    1. Убрать комментарий в tasks.json в -mwindows"
*/

// Подключение библиотеки SDL
#include "SDL2/i686-w64-mingw32/include/SDL2/SDL.h"

#include <windows.h>
#include <iostream>

using namespace std;

typedef unsigned short us;

constexpr us SCREEN_WIDTH = 640;
constexpr us SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{
  SDL_Window *window = NULL;

  SDL_Surface *screenSurface = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
  else
  {
    window = SDL_CreateWindow("SDL Tutorial",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    if (window == NULL)
      cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
    else
    {
      screenSurface = SDL_GetWindowSurface(window);
      SDL_UpdateWindowSurface(window);
      SDL_Delay(2000);
      
      SDL_DestroyWindow(window);
      SDL_Quit();

      return 0;
    }
  }

  return 0;
}
