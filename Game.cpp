#include "Game.h"

Game::Game()
{
  cout << "START";

  playing = true;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    this->~Game();
  }
  else
  {
    TTF_Init();
    auto request = SDL_GetDesktopDisplayMode(0, &dMode);

    mode = MENU;
    player = NONE;

    // 16 / 9 - display aspect ratio for every screens
    screen.set(dMode.w / 2, (dMode.w * 9 / 16) / 2);

    playButton.set(screen.width / 3,
                   screen.width * 2 / 3,
                   screen.height * 10 / 18,
                   screen.height * 13 / 18);

    exitButton.set(screen.width / 3,
                   screen.width * 2 / 3,
                   screen.height * 14 / 18,
                   screen.height * 17 / 18);

    createGameButton.set(screen.width / 4,
                         screen.width * 3 / 4,
                         screen.height * 10 / 18,
                         screen.height * 13 / 18);

    joinGameButton.set(screen.width / 4,
                       screen.width * 3 / 4,
                       screen.height * 14 / 18,
                       screen.height * 17 / 18);

    {
      srand(time(0));
      randomSide = rand() % 2;
      for (us i = 0; i < N; i++)
        for (us j = 0; j < N; j++)
        {
          if (i < 3)
            board[i][j] = ((i + j) % 2 == 0) ? WHITE_FREE : ((randomSide) ? BLACK_USUAL : WHITE_USUAL);
          else if (i > 4)
            board[i][j] = ((i + j) % 2 == 0) ? WHITE_FREE : ((randomSide) ? WHITE_USUAL : BLACK_USUAL);
          else
            board[i][j] = ((i + j) % 2 == 0) ? WHITE_FREE : BLACK_FREE;

          cells[i][j].set((screen.width * 7 / 12) / 2 + j * screen.width * 5 / 96,
                          (screen.width * 7 / 12) / 2 + (j + 1) * screen.width * 5 / 96,
                          (screen.height * 7 / 27) / 2 + i * screen.height * 20 / 216,
                          (screen.height * 7 / 27) / 2 + (i + 1) * screen.height * 20 / 216);
        }

      for (us i = 0; i < N; i++)
        for (us j = 0; j < N; j++)
          tmpBoard[i][j] = board[i][j];
    }

    window = SDL_CreateWindow("SDL Tutorial",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen.width,
                              screen.height - 30,
                              SDL_WINDOW_SHOWN | /* SDL_WINDOW_BORDERLESS | */ SDL_WINDOW_ALLOW_HIGHDPI /*| SDL_WINDOW_FULLSCREEN*/);

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

  backgroundMenuTexture = IMG_LoadTexture(ren, "textures/menu.jpg");
  backgroundChoiceTexture = IMG_LoadTexture(ren, "textures/showChoice.jpg");
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
  cellTexture[2] = IMG_LoadTexture(ren, "textures/BLACK_QUEEN.jpg");
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

void Game::showChoice()
{
  SDL_RenderClear(ren);

  // textures
  SDL_RenderCopy(ren, backgroundChoiceTexture, NULL, &backgroundRect);

  SDL_RenderPresent(ren);
}

void Game::showGame()
{

  SDL_RenderClear(ren);

  // textures
  SDL_RenderCopy(ren, backgroundGameTexture, NULL, &backgroundRect);

  if (x == -1 || y == -1)
  {
    for (us i = 0; i < N; i++)
      for (us j = 0; j < N; j++)
        SDL_RenderCopy(ren,
                       cellTexture[tmpBoard[i][j]],
                       NULL, (player == SERVER) ? &boardRect[i][j] : &boardRect[(N - 1) - i][(N - 1) - j]);
  }
  else
  {
    for (us i = 0; i < N; i++)
      cout << "b -> " << board[3][i] << ' ';
    cout << endl;

    for (us i = 0; i < N; i++)
      for (us j = 0; j < N; j++)
        tmpBoard[i][j] = board[i][j];

    for (us i = 0; i < N; i++)
      cout << "t ->" << tmpBoard[3][i] << ' ';
    cout << endl;

    steps = 0;
    findWay(x, y);

    for (us i = 0; i < N; i++)
      for (us j = 0; j < N; j++)
        SDL_RenderCopy(ren,
                       cellTexture[board[i][j]],
                       NULL, (player == SERVER) ? &boardRect[i][j] : &boardRect[(N - 1) - i][(N - 1) - j]);
  }

  SDL_RenderPresent(ren);
}

void Game::findWay(us i, us j)
{
  if (board[i][j] == BLACK_USUAL)
  {
    if (i + 1 < N)
    {
      if (j + 1 < N)
      {
        if (board[i + 1][j + 1] == BLACK_FREE)
        {
          board[i + 1][j + 1] = CAN_DO_STEP;
        }
        else if (board[i + 1][j + 1] == WHITE_USUAL || board[i + 1][j + 1] == WHITE_QUEEN)
        {
          if (i + 2 < N)
          {
            if (j + 2 < N)
            {
              if (board[i + 2][j + 2] == BLACK_FREE)
              {
                if (!steps)
                  board[i + 2][j + 2] = CAN_DO_STEP;
                steps++;
                findWay(i + 2, j + 2);
              }
            }
          }
        }
      }
      if (j - 1 >= 0)
      {
        if (board[i + 1][j - 1] == BLACK_FREE)
        {
          board[i + 1][j - 1] = CAN_DO_STEP;
        }
        else if (board[i + 1][j - 1] == WHITE_USUAL || board[i + 1][j - 1] == WHITE_QUEEN)
        {
          if (i + 2 < N)
          {
            if (j - 2 >= 0)
            {
              if (board[i + 2][j - 2] == BLACK_FREE)
              {
                if (!steps)
                  board[i + 2][j - 2] = CAN_DO_STEP;
                steps++;
                findWay(i + 2, j - 2);
              }
            }
          }
        }
      }
    }
    if (i - 1 >= 0)
    {
      if (j + 1 < N)
      {
        if (board[i - 1][j + 1] == BLACK_FREE)
        {
          board[i - 1][j + 1] = CAN_DO_STEP;
        }
        else if (board[i - 1][j + 1] == WHITE_USUAL || board[i - 1][j + 1] == WHITE_QUEEN)
        {
          if (i - 2 >= 0)
          {
            if (j + 2 < N)
            {
              if (board[i - 2][j + 2] == BLACK_FREE)
              {
                if (!steps)
                  board[i - 2][j + 2] = CAN_DO_STEP;
                steps++;
                findWay(i - 2, j + 2);
              }
            }
          }
        }
      }
      if (j - 1 >= 0)
      {
        if (board[i - 1][j - 1] == BLACK_FREE)
        {
          board[i - 1][j - 1] = CAN_DO_STEP;
        }
        else if (board[i - 1][j - 1] == WHITE_USUAL || board[i - 1][j - 1] == WHITE_QUEEN)
        {
          if (i - 2 >= 0)
          {
            if (j - 2 >= 0)
            {
              if (board[i - 2][j - 2] == BLACK_FREE)
              {
                if (!steps)
                  board[i - 2][j - 2] = CAN_DO_STEP;
                steps++;
                findWay(i - 2, j - 2);
              }
            }
          }
        }
      }
    }
  }
}

void Game::startServer()
{
  string s = "Hello World";
  server.sendData((void *)s.c_str(), s.size() + 1);
}

void Game::joinServer()
{
  void *data = NULL;
  while (1)
  {
    client.recvData();
    data = client.getData();
    if (data != NULL)
      break;
  }
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
          if (mode == MENU)
          {
            if (playButton.isPressed(&event))
              mode = CHOICE;
            else if (exitButton.isPressed(&event))
              playing = false;
          }
          else if (mode == CHOICE)
          {
            if (createGameButton.isPressed(&event))
            {
              player = SERVER;
              mode = GAME;
              (board[N - 1][0] == WHITE_USUAL) ? move = true : move = false;
              // startServer();
            }
            else if (joinGameButton.isPressed(&event))
            {
              player = CLIENT;
              mode = GAME;
              // (board[N - 1][0] == WHITE_USUAL) ? move = true : move = false;
              // joinServer();
            }
          }
          else if (mode == GAME)
          {
            x = -1;
            y = -1;
            for (us i = 0; i < N; i++)
              for (us j = 0; j < N; j++)
                if (cells[i][j].isPressed(&event) &&
                    (board[i][j] == BLACK_USUAL || BLACK_QUEEN || WHITE_USUAL || WHITE_QUEEN) &&
                    move)
                {
                  for (us i = 0; i < N; i++)
                    for (us j = 0; j < N; j++)
                      board[i][j] = tmpBoard[i][j];
                  x = i;
                  y = j;
                  break;
                }
          }
        }
    }

    switch (mode)
    {
    case MENU:
      showMenu();
      break;
    case CHOICE:
      showChoice();
      break;
    case GAME:
      showGame();
      break;
    default:
      break;
    }
  }
}

Game::~Game()
{
  // TODO SDL_DestroyTexture()

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(window);
  SDL_Quit();
}