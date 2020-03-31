#include "server.h"

Server::Server()
{
  SDLNet_Init();  
}

void Server::sendData(const void *data, us dataSize)
{
  SDLNet_ResolveHost(&ip, NULL, Port);

  server = SDLNet_TCP_Open(&ip);

  while (true)
  {
    client = SDLNet_TCP_Accept(server);
    if (client)
    {
      SDLNet_TCP_Send(client, data, dataSize);
      SDLNet_TCP_Close(client);
      break;
    }
  }
  
  SDLNet_TCP_Close(server);
}

void Server::incPort()
{
  Port++;
}

void Server::decPort()
{
  Port--;
}

Server::~Server()
{
  SDLNet_Quit();
}
