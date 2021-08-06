#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char** argv)
{
  int p[2];
  char buf[512] = { 0 };
  pipe(p);
  if (fork() == 0 && read(p[0], buf, sizeof(buf) - 1) == 1) {
    printf("%d: received ping\n", getpid());
    // printf("%s\n", buf);
    write(p[1], "c", 1);
    close(p[0]);
    close(p[1]);
    exit(0);
  }
  write(p[1], "p", 1);
  wait(0);
  if (read(p[0], buf, sizeof(buf) - 1) == 1)
    printf("%d: received pong\n", getpid());
  // printf("%s\n", buf);
  close(p[0]);
  close(p[1]);
  exit(0);
}
