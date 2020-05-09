#include <iostream>

#include "../SDL2/i686-w64-mingw32/include/SDL2/SDL_net.h"

using namespace std;

#ifndef CLIENT_H
#define CLIENT_H

#define MAX_DATASIZE 400

typedef unsigned short us;

class Client
{
private:
  us Port = 740;

  IPaddress ip;
  TCPsocket client;

  
public:
  Client();

  void * recv();

  ~Client();
};

#endif // CLIENT_H