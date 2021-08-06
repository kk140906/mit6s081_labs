#include "kernel/types.h"
#include "user/user.h"

#define MAX_PRIMES 35


void pipeline(int fd)
{
  // 第一次读取管道的值作为该进程的初始prime
  int prime;
  if (read(fd, &prime, sizeof(int)) <= 0) {
    close(fd);
    exit(1);
  }
  printf("prime %d\n", prime);

  int p[2] = { -1 };
  pipe(p);

  if (fork() == 0) {
    close(p[1]);
    pipeline(p[0]);
    exit(0);
  }

  close(p[0]);
  int val;
  while (read(fd, &val, sizeof(int))) {
    if (val % prime == 0)
      continue;
    write(p[1], &val, sizeof(int));
  }
  close(fd);
  close(p[1]);
  wait(0);
  exit(0);
}

int main(int argc, char** argv)
{
  int p[2] = { -1 };
  pipe(p);

  if (fork() == 0) {
    // xv6 资源不多，能提前关闭的文件描述符都需要提前关闭
    close(p[1]);
    // p[0] 在管线中关闭
    pipeline(p[0]);
    exit(0);
  }

  close(p[0]);
  for (int i = 2; i <= MAX_PRIMES; ++i) {
    write(p[1], &i, sizeof(int));
  }
  close(p[1]);
  wait(0);
  exit(0);
}
