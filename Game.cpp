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

    {
      srand(time(0));
      auto randomSide = rand() % 2;
      for (us i = 0; i < N; i++)
        for (us j = 0; j < N; j++)
        {
          if (i < 3)
            board[i][j] = ((i + j) % 2 == 0) ? WHITE_FREE : ((randomSide) ? BLACK_USUAL : WHITE_USUAL);
          else if (i > 4)
            board[i][j] = ((i + j) % 2 == 0) ? WHITE_FREE : ((randomSide) ? WHITE_USUAL : BLACK_USUAL);
          else
            board[i][j] = ((i + j) % 2 == 0) ? WHITE_FREE : BLACK_FREE;

          cells[i][j].set((screen.width - 800) / 2 + j * 100,
                          (screen.width - 800) / 2 + (j + 1) * 100,
                          (screen.height - 800) / 2 + i * 100,
                          (screen.height - 800) / 2 + (i + 1) * 100);
        }
    }

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

  backgroundMenuTexture = IMG_LoadTexture(ren, "textures/background.jpg");

  backgroundGameTexture = IMG_LoadTexture(ren, "textures/game.jpg");

  // Creating rectangles for the cells on board
  {
    for (us i = 0; i < N; i++)
      for (us j = 0; j < N; j++)
      {
        boardRect[i][j].x = cells[i][j].x.first;
        boardRect[i][j].y = cells[i][j].y.first;
        boardRect[i][j].w = cells[i][j].x.second - cells[i][j].x.first;
        boardRect[i][j].h = cells[i][j].y.second - cells[i][j].y.first;
      }
  }

  cellTexture[0] = IMG_LoadTexture(ren, "textures/BLACK_FREE.jpg");
  cellTexture[1] = IMG_LoadTexture(ren, "textures/BLACK_USUAL.jpg");
  cellTexture[2] = IMG_LoadTexture(ren, "textures/BLACK_QUENN.jpg");
  cellTexture[3] = IMG_LoadTexture(ren, "textures/WHITE_FREE.jpg");
  cellTexture[4] = IMG_LoadTexture(ren, "textures/WHITE_USUAL.jpg");
  cellTexture[5] = IMG_LoadTexture(ren, "textures/WHITE_QUEEN.jpg");
  cellTexture[6] = IMG_LoadTexture(ren, "textures/CAN_DO_STEP.jpg");
  cellTexture[7] = IMG_LoadTexture(ren, "textures/CAN_GET_CELL.jpg");
}

void Game::showMenu()
{
  SDL_RenderClear(ren);

  // textures
  SDL_RenderCopy(ren, backgroundMenuTexture, NULL, &backgroundRect);

  SDL_RenderPresent(ren);
}

void Game::showGame()
{
  SDL_RenderClear(ren);

  // textures
  SDL_RenderCopy(ren, backgroundGameTexture, NULL, &backgroundRect);

  for (us i = 0; i < N; i++)
    for (us j = 0; j < N; j++)
      SDL_RenderCopy(ren, cellTexture[board[i][j]], NULL, &boardRect[i][j]);

  SDL_RenderPresent(ren);
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
            mode = GAME;
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