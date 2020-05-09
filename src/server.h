#include <iostream>

#include "../SDL2/i686-w64-mingw32/include/SDL2/SDL_net.h"

using namespace std;

#ifndef SERVER_H
#define SERVER_H

typedef unsigned short us;

class Server
{
private:
  us Port = 740;

  IPaddress ip;
  TCPsocket server;
  TCPsocket client;

public:
  Server();

  void send(const void *data, us size);

  ~Server();
};

#endif // SERVER_H