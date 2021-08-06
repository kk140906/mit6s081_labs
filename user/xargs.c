#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int getline(char *buf) {
  char c;
  char *s = buf;
  while (read(0, &c, 1) == 1 && c != '\n') {
    *buf++ = c;
  }
  return strlen(s);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(2, "xargs take one argument at least.");
    exit(1);
  }

  char *args[MAXARG];

  for (int i = 0; i < argc - 1; ++i) {
    args[i] = argv[i + 1];
  }

  char buf[MAXPATH] = {0};

  while (getline(buf)) {
    args[argc - 1] = buf;
    args[argc] = 0;
    if (fork() == 0) {
      exec(argv[1], args);
      exit(0);
    }
    wait(0);
    memset(buf, 0, MAXPATH);
  }
  exit(0);
}
