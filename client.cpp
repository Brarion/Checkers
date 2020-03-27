#include "client.h"

Client::Client()
{
  SDLNet_Init();

  SDLNet_ResolveHost(&ip, "localhost", Port);
}

void Client::recvData()
{
  client = SDLNet_TCP_Open(&ip);
  SDLNet_TCP_Recv(client, data, 100);
  SDLNet_TCP_Close(client);
}

void *Client::getData()
{
  return data;
}

Client::~Client()
{
  SDLNet_Quit();
}
