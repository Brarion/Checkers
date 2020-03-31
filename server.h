#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_net.h"

#include <iostream>

using namespace std;

typedef unsigned short us;

class Server
{
private:
  unsigned short Port = 740;

  IPaddress ip;
  TCPsocket server;
  TCPsocket client;

public:
  Server();

  void sendData(const void *data, us dataSize);

  void incPort();
  void decPort();

  ~Server();
};
