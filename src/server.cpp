#include "server.h"

Server::Server()
{
  SDLNet_Init();
}

void Server::send(const void *data, us size)
{
  SDLNet_ResolveHost(&ip, NULL, Port);

  server = SDLNet_TCP_Open(&ip);

  while (true)
  {
    client = SDLNet_TCP_Accept(server);

    if (client)
    {
      SDLNet_TCP_Send(client, data, size);
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