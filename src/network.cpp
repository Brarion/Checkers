#include "network.h"

Network::Network()
{
}

void Network::send(const void *data, us size)
{
  server.send(data, size);
}

void *Network::recv()
{
  void *data = NULL;
  while (data == NULL)
    data = client.recv();

  return data;
}

Network::~Network()
{
}