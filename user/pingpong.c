#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);


  if (fork() == 0){
    close(p1[1]);
    char buf[5];
    int id = getpid();
    read(p1[0], buf, 4);
    buf[4] = '\0';
    printf("%d: received %s\n", id, buf);

    close(p2[0]);
    write(p2[1], "pong", 4);
    exit(0);
  }
  else {
    close(p1[0]);
    write(p1[1], "ping", 4);

    char buf[5];
    close(p2[1]);
    read(p2[0], buf, 4);
    buf[4] = '\0';

    int id = getpid();
    printf("%d: received %s\n", id, buf);
    exit(0);
  }
}
