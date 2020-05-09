#include <iostream>

#include "../SDL2/i686-w64-mingw32/include/SDL2/SDL_net.h"

#include "server.h"
#include "client.h"

#ifndef NETWORK_H
#define NETWORK_H

typedef unsigned short us;

class Network
{
public:
  Network();

  Client client;
  Server server;

  void send(const void *data, us size);
  void * recv();

  ~Network();
};

#endif // NETWORK_H