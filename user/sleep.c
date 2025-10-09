#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc != 2){
    fprintf(2, "Provide one argument\n");
    exit(1);
  }
  int sleepTime = atoi(argv[1]);
  sleep(sleepTime);
  printf("Slept for %d ticks\n", sleepTime);
  exit(0);
}
