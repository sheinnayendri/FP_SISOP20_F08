#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf [1000000];

int main (int argc, char *argv[]) {
  int fd, i, j, n, lines = 0, toPrint, c = 0, startLine;
  if(argc <= 1) {
    printf(1, "tail: need at least 1 argument!");
    exit();
  }
  for(j = 1; j < argc; j++)
  {
    lines = 0, toPrint = 0, c = 0, startLine = 0;
    if((fd = open(argv[j], O_RDWR)) < 0) {
      printf(1, "tail: cannot open %s for reading\n", argv[j]);
      exit();
    }
    printf(1, "==> %s <==\n", argv[j]);
    char *buffer;
    buffer = (char*) malloc(1000000 * sizeof(char));
    int buffSize = 0;
    while((n = read(fd, buf, sizeof(buf))) > 0)
    {
      for(i = 0; i < n; i++)
      {
        buffer[buffSize] = (char)buf[i];
        buffSize++;
        if(buf[i] == '\n') lines++;
      }
    }
    if(n < 0) {
      printf(1, "tail: read error\n");
      exit();
    }
    if(lines < 10) toPrint = lines + 1;
    else toPrint = 10;
    startLine = lines - toPrint + 1;
    for(i = 0; i < buffSize; i++)
    {
      if(c >= startLine) printf(1, "%c", buffer[i]);
      if(buffer[i] == '\n') c++;
    }
    printf(1, "\n");
    if(j != argc - 1) printf(1, "\n");
  }
  exit();
}
