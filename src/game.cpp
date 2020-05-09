#include "game.h"

Game::Game()
{
  playing = true;
  mode = MENU;
  player = NONE;
  SDL_Init(SDL_INIT_EVERYTHING);

  TTF_Init();

  auto request = SDL_GetDesktopDisplayMode(0, &dMode);

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

  for (us i = 0; i < N - 2; i++)
    for (us j = 0; j < N - 2; j++)
      cells[i][j].set((screen.width * 7 / 12) / 2 + j * screen.width * 5 / 96,
                      (screen.width * 7 / 12) / 2 + (j + 1) * screen.width * 5 / 96,
                      (screen.height * 7 / 27) / 2 + i * screen.height * 20 / 216,
                      (screen.height * 7 / 27) / 2 + (i + 1) * screen.height * 20 / 216);

  window = SDL_CreateWindow("Шашки",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            screen.width,
                            screen.height - 30,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

  ren = SDL_CreateRenderer(window,
                           -1,
                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  this->loadTextures();
  this->play();
}

void Game::loadTextures()
{
  font = TTF_OpenFont("fonts/courier.ttf", 28);
  // textRect = {0, 0, 300, 28};
  textRect.x = 0;
  textRect.y = 0;
  textRect.w = screen.width / 4;
  textRect.h = 28;

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
    for (us i = 0; i < N - 2; i++)
      for (us j = 0; j < N - 2; j++)
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

void Game::play()
{
  SDL_Event event;
  flag = true;

  while (playing)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        playing = false;
      else if (event.type == SDL_MOUSEBUTTONDOWN)
      {

        switch (mode)
        {
        case MENU:
          if (playButton.isPressed(&event))
            mode = CHOICE;
          else if (exitButton.isPressed(&event))
            playing = false;
          break;

        case CHOICE:
          if (createGameButton.isPressed(&event))
          {
            player = HOST;

            this->show();

            mode = GAME;
            network.send((void *)board.getBoard(), sizeof(Cell) * N * N);

            myStep = board.getCell((N - 2) - 1, 0) == WHITE_USUAL;
            (myStep) ? side = WHITE : side = BLACK;
          }
          else if (joinGameButton.isPressed(&event))
          {
            player = GUEST;

            this->show();

            mode = GAME;
            board.setBoard(network.recv());

            myStep = board.getCell((N - 2) - 1, 0) == WHITE_USUAL;
            (myStep) ? side = WHITE : side = BLACK;
          }
          break;

        case GAME:

          if (myStep)
          {

            us y = ((us)((event.button.x - (screen.width * 7 / 12) / 2) / (screen.width * 5 / 96))) & 0x07;
            us x = ((us)((event.button.y - (screen.height * 7 / 27) / 2) / (screen.width * 5 / 96))) & 0x07;

            if (cells[x][y].isPressed(&event))
            {
              if (flag && (((side == BLACK) && // Черные ходят
                            (board.getCell(x, y) == BLACK_USUAL ||
                             board.getCell(x, y) == BLACK_QUEEN)) ||
                           ((side == WHITE) && // Белые ходят
                            (board.getCell(x, y) == WHITE_USUAL ||
                             board.getCell(x, y) == WHITE_QUEEN))))
              {
                if (board.getCell(x, y) == BLACK_QUEEN ||
                    board.getCell(x, y) == WHITE_QUEEN)
                  isQueen = true;
                else if (board.getCell(x, y) == BLACK_USUAL ||
                         board.getCell(x, y) == WHITE_USUAL)
                  isQueen = false;
                if (flag)
                {
                  board.clear();
                  board.findWay(x, y, side, isQueen, false);
                  currentChecker = pair<us, us>(x, y);
                }
              }
              else if (board.getCell(x, y) == FINAL_STEP)
              {
                uc tmp = board.move(currentChecker.first, currentChecker.second, x, y);
                currentChecker = pair<us, us>(x, y);
                board.clear();

                if (tmp == 1)
                {
                  this->show();
                  mode = BLACK_WIN;
                  this->show();
                }
                else if (tmp == 2)
                {
                  this->show();
                  mode = WHITE_WIN;
                  this->show();
                }

                myStep = false;
                this->show();
                network.send((void *)board.getBoard(), sizeof(Cell) * N * N);
              }
              else if (board.getCell(x, y) == INTERMEDIATE_STEP)
              {
                if (!isQueen && abs(currentChecker.first - x) != 2 && abs(currentChecker.second - y) != 2)
                  break;

                board.clear();
                uc tmp = board.move(currentChecker.first, currentChecker.second, x, y);
                currentChecker = pair<us, us>(x, y);
                board.findWay(currentChecker.first, currentChecker.second, side, isQueen, true);

                flag = true;
                for (us i = 0; i < N - 2 && flag; i++)
                  for (us j = 0; j < N - 2 && flag; j++)
                    if (board.getCell(i, j) == INTERMEDIATE_STEP || board.getCell(i, j) == FINAL_STEP)
                      flag = false;

                if (tmp == 1)
                {
                  this->show();
                  mode = BLACK_WIN;
                  this->show();
                }
                else if (tmp == 2)
                {
                  this->show();
                  mode = WHITE_WIN;
                  this->show();
                }

                if (flag)
                {
                  myStep = false;
                  this->show();
                  network.send((void *)board.getBoard(), sizeof(Cell) * N * N);
                }

                this->show();
              }
              else
                this->show();
            }
            else
              this->show();

            if (board.getCantMove(side) == 1)
            {
              this->show();
              mode = BLACK_WIN;
              this->show();
            }
            else if (board.getCantMove(side) == 2)
            {
              this->show();
              mode = WHITE_WIN;
              this->show();
            }
          }
          else
          {
            board.setBoard(network.recv());
            myStep = true;

            if (board.getCantMove(side) == 1)
            {
              this->show();
              mode = BLACK_WIN;
              this->show();
            }
            else if (board.getCantMove(side) == 2)
            {
              this->show();
              mode = WHITE_WIN;
              this->show();
            }
          }

          break;
        default:
          break;
        }
      }
    }

    this->show();
  }
}

void Game::show()
{
  switch (mode)
  {
  case MENU:
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, backgroundMenuTexture, NULL, &backgroundRect);
    SDL_RenderPresent(ren);
    break;

  case CHOICE:
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, backgroundChoiceTexture, NULL, &backgroundRect);

    if (player != NONE)
      SDL_RenderCopy(ren,
                     SDL_CreateTextureFromSurface(ren,
                                                  TTF_RenderUNICODE_Blended(font,
                                                                            (Uint16 *)(L"Ожидание     "),
                                                                            {0, 0, 0})),
                     NULL,
                     &textRect);

    SDL_RenderPresent(ren);
    break;

  case GAME:
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, backgroundGameTexture, NULL, &backgroundRect);

    if (myStep)
      SDL_RenderCopy(ren,
                     SDL_CreateTextureFromSurface(ren,
                                                  TTF_RenderUNICODE_Blended(font,
                                                                            (Uint16 *)(L"Ваш ход      "),
                                                                            {0, 0, 0})),
                     NULL,
                     &textRect);
    else
      SDL_RenderCopy(ren,
                     SDL_CreateTextureFromSurface(ren,
                                                  TTF_RenderUNICODE_Blended(font,
                                                                            (Uint16 *)(L"Ход соперника"),
                                                                            {0, 0, 0})),
                     NULL,
                     &textRect);

    for (us i = 0; i < N - 2; i++)
      for (us j = 0; j < N - 2; j++)
        SDL_RenderCopy(ren, cellTexture[board.getCell(i, j) - 1], NULL, &boardRect[i][j]);

    SDL_RenderPresent(ren);
    break;

  case WHITE_WIN:
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, backgroundGameTexture, NULL, &backgroundRect);

    SDL_RenderCopy(ren,
                   SDL_CreateTextureFromSurface(ren,
                                                TTF_RenderUNICODE_Blended(font,
                                                                          (Uint16 *)(L"Победа белых "),
                                                                          {0, 0, 0})),
                   NULL,
                   &textRect);

    for (us i = 0; i < N - 2; i++)
      for (us j = 0; j < N - 2; j++)
        SDL_RenderCopy(ren, cellTexture[board.getCell(i, j) - 1], NULL, &boardRect[i][j]);

    SDL_RenderPresent(ren);
    break;

  case BLACK_WIN:
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, backgroundGameTexture, NULL, &backgroundRect);

    SDL_RenderCopy(ren,
                   SDL_CreateTextureFromSurface(ren,
                                                TTF_RenderUNICODE_Blended(font,
                                                                          (Uint16 *)(L"Победа черных"),
                                                                          {0, 0, 0})),
                   NULL,
                   &textRect);

    for (us i = 0; i < N - 2; i++)
      for (us j = 0; j < N - 2; j++)
        SDL_RenderCopy(ren, cellTexture[board.getCell(i, j) - 1], NULL, &boardRect[i][j]);

    SDL_RenderPresent(ren);
    break;

  default:
    break;
  }
}

Game::~Game()
{
  // TODO SDL_DestroyTexture()

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(window);
  SDL_Quit();
}