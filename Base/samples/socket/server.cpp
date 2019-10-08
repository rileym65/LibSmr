#include <stdio.h>
#include <SmrFramework.h>

using namespace SmrFramework;

int main(int argc,char **argv) {
  UInt32 status;
  char msg[1024];
  Socket* sock;
  Socket* client;
    sock = new Socket();
    sock->ReuseAddress(true);
    sock->Linger(true, 30);
    sock->Bind(30500);
    sock->Listen();
    while (1) {
      try {
        client = sock->Accept();
        status = client->Receive((Byte*)msg, 10);
        if (status > 0) {
          printf("Message received %d %d %d\r\n",msg[0],msg[1],msg[2]);
          msg[1] = msg[1] << 1;
          msg[2] = msg[2] << 1;
          client->Send((Byte*)msg, 3);
          }
        }
      catch (InvalidOpException e) {
        printf("Exception: %s\n",e.Message());
        }
      }
  delete(client);
  delete(sock);

  }

