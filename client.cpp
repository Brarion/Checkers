#include "client.h"

Client::Client()
{
  SDLNet_Init();
}

void Client::recvData()
{
  SDLNet_ResolveHost(&ip, "localhost", Port);

  client = SDLNet_TCP_Open(&ip);
  while (client == NULL)
  {
    client = SDLNet_TCP_Open(&ip);
  }
  SDLNet_TCP_Recv(client, data, MAX_DATASIZE);
  SDLNet_TCP_Close(client);
}

void *Client::getData()
{
  return data;
}

void Client::incPort()
{
  Port++;
}

void Client::decPort()
{
  Port--;
}

Client::~Client()
{
  SDLNet_Quit();
}
