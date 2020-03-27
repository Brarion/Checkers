#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_net.h"

#include <iostream>

#define Port 740 // free port (see wiki)

using namespace std;

typedef unsigned short us;

class Client
{
private:
  IPaddress ip;
  TCPsocket client;

  char data[100];

public:
  Client();

  void recvData();

  void *getData();

  ~Client();
};
