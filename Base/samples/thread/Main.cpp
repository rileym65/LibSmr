#include <SmrFramework.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace SmrFramework;

int shared;
bool done[2];

class MyThread : public Thread {
  public:
    virtual void execute(void* arg);
  };

void MyThread::execute(void* arg) {
  int i;
  int id;
  id = *((int*)arg);
  sleep(1);
  printf("Starting thread\n");
  for (i=0; i<10; i++) {
    printf("%d: %d: %d\n",shared,id,i);
    sleep(1);
    }
  done[id-1] = true;
  }

int main(int argc, char** argv) {
  int id1;
  int id2;
  shared = 456;
  MyThread* th1;
  MyThread* th2;
  printf("Starting\n");
  id1 = 1;
  id2 = 2;
  done[0] = false;
  done[1] = false;
  th1 = new MyThread();
  th2 = new MyThread();
  th1->Start(&id1);
  usleep(500000);
  th2->Start(&id2);
  printf("Thread 1 id: %x\n",th1->ThreadId());
  printf("Thread 2 id: %x\n",th2->ThreadId());
  while (!done[0] || !done[1]) ;
  }

