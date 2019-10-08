#include <stdio.h>
#include <SmrFramework.h>

using namespace SmrFramework;

int main(int argc,char **argv) {
  Socket* sock;
  char    msg[1024];
  UInt32  status;
  
  try {
    sock = new Socket();
    sock->Connect("127.0.0.1", 30500);
printf("Connected to: %d\n",sock->Port());
    msg[0] = 3;
    msg[1] = 20;
    msg[2] = 5;
    sock->Send((Byte*)msg, 3);
    status = sock->Receive((Byte*)msg, 10);
    printf("received: %d %d %d\r\n",msg[0],msg[1],msg[2]);
    }
  catch (InvalidOpException e) {
    printf("Exception: %s\n",e.Message());
    }

  delete(sock);


  }

