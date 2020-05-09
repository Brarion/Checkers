#include "client.h"

Client::Client()
{
  SDLNet_Init();
}

void *Client::recv()
{
  static char data[MAX_DATASIZE];

  SDLNet_ResolveHost(&ip, "localhost", Port);

  client = SDLNet_TCP_Open(&ip);
  
  while (client == NULL)
    client = SDLNet_TCP_Open(&ip);

  SDLNet_TCP_Recv(client, data, MAX_DATASIZE);
  SDLNet_TCP_Close(client);

  return data;
}

Client::~Client()
{
  SDLNet_Quit();
}