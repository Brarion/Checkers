#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_net.h"

#include <iostream>

#define Port 740 // free port (see wiki)

using namespace std;

typedef unsigned short us;

class Server
{
private:
  IPaddress ip;
  TCPsocket server;
  TCPsocket client;

public:
  Server();

  void sendData(const void *data, us dataSize);

  ~Server();
};
