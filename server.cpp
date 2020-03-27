#include "server.h"

Server::Server()
{
  SDLNet_Init();

  SDLNet_ResolveHost(&ip, NULL, Port);
}

void Server::sendData(const void *data, us dataSize)
{
  server = SDLNet_TCP_Open(&ip);
  while (true)
  {
    client = SDLNet_TCP_Accept(server);
    if (client)
    {
      SDLNet_TCP_Send(client, data, 100);
      SDLNet_TCP_Close(client);
      break;
    }
  }
  SDLNet_TCP_Close(server);
}

Server::~Server()
{
  SDLNet_Quit();
}
