#include "kernel/types.h"
#include "user/user.h"

int main(int argc ,char **argv) {
  if (argc != 2) {
    const char *info = "sleep take one argument.\n";
    write(2,info,strlen(info));
    exit(1);
  }
  int ticks = atoi(argv[1]);
  sleep(ticks);
  exit(0);
}
