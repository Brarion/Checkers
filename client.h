#include "SDL2/i686-w64-mingw32/include/SDL2/SDL_net.h"

#include <iostream>

#define MAX_DATASIZE 256

using namespace std;

typedef unsigned short us;

class Client
{
private:
  unsigned short Port = 740;

  IPaddress ip;
  TCPsocket client;

  char data[MAX_DATASIZE];

public:
  Client();

  void recvData();

  void *getData();

  void incPort();
  void decPort();

  ~Client();
};
