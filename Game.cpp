#include "Game.h"

Game::Game()
{
  playing = true;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    this->~Game();
  }
  else
  {
    auto request = SDL_GetDesktopDisplayMode(0, &dMode);

    mode = MENU;

    // 16 / 9 - display aspect ratio for every screens
    screen.set(dMode.w, dMode.w * 9 / 16);

    playButton.set(screen.width / 3,
                   screen.width * 2 / 3,
                   screen.height * 10 / 18,
                   screen.height * 13 / 18);

    exitButton.set(screen.width / 3,
                   screen.width * 2 / 3,
                   screen.height * 14 / 18,
                   screen.height * 17 / 18);

    window = SDL_CreateWindow("SDL Tutorial",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen.width,
                              screen.height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN);

    if (window == NULL)
    {
      cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
      this->~Game();
    }
    else
    {
      ren = SDL_CreateRenderer(window,
                               -1,
                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

      if (ren == NULL)
      {
        cout << "SDL_CreateRednerer error! SDL_Error: " << SDL_GetError() << endl;
        this->~Game();
      }
      else
      {
        this->loadTextures();
        this->play();
      }
    }
  }
}

void Game::loadTextures()
{
  // Creating rectangle for the background texture
  {
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = screen.width;
    backgroundRect.h = screen.height;
  }

  backgroundTexture = IMG_LoadTexture(ren, "textures/background.jpg");
}

void Game::showMenu()
{
  SDL_RenderClear(ren);

  // textures
  SDL_RenderCopy(ren, backgroundTexture, NULL, &backgroundRect);
  // SDL_RenderCopy(ren, chessBoardTexture, NULL, &chessBoardRect);

  SDL_RenderPresent(ren);
}

void Game::showGame()
{

}

void Game::play()
{
  SDL_Event event;
  while (playing)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        playing = false;
      else if (event.type == SDL_MOUSEBUTTONDOWN)
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          if (playButton.isPressed(&event))
          {
            // PlayGame
          }
          else if (exitButton.isPressed(&event))
            playing = false;
      }
    }
    (mode == MENU) ? showMenu() : showGame();
  }
}

Game::~Game()
{
  // TODO SDL_DestroyTexture()

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(window);
  SDL_Quit();
}