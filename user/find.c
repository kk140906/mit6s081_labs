#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PATH_LEN 256
typedef enum { false,
  true } bool;
bool match(const char* dirs, const char* file)
{
  const char* p = dirs + strlen(dirs);
  char formated_dirs[MAX_PATH_LEN];
  while (*p != '/')
    p--;
  strcpy(formated_dirs, ++p);
  return !strcmp(formated_dirs, file);
}

void find(char* dir, const char* file)
{
  int fd;
  if ((fd = open(dir, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", dir);
    return;
  }

  struct stat st;
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", dir);
    close(fd);
    return;
  }

  char dirs[MAX_PATH_LEN] = { 0 };
  struct dirent de;
  char* p;
  switch (st.type) {
  case T_DIR:
    strcpy(dirs, dir);
    p = dirs + strlen(dirs);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(dirs, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
      }
      if (st.type == T_FILE && match(dirs, file)) {
        printf("%s\n", dirs);
      } else if (st.type == T_DIR && dirs[strlen(dirs) - 1] != '.') {
        find(dirs, file);
      }
    }
    break;
  default:
    break;
  }
}

int main(int argc, char** argv)
{
  if (argc != 3) {
    fprintf(2, "usage: find [dir] [file].");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
