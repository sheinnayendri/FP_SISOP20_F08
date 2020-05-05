#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[1000000];

int main (int argc, char *argv[]) {
  int fd, fd2, i, n;
  if(argc <= 2) {
    printf(1, "cmp: need 2 arguments!");
    exit();
  }
  
  if((fd = open(argv[1], O_RDWR)) < 0) {
    printf(1, "cmp: cannot open %s for reading\n", argv[1]);
    exit();
  }
  char *buffer;
  buffer = (char*) malloc(1000000 * sizeof(char));
  int buffSize = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0)
  {
    for(i = 0; i < n; i++)
    {
      buffer[buffSize] = (char)buf[i];
      buffSize++;
    }
  }
  if(n < 0) {
    printf(1, "cmp: read error\n");
    exit();
  }
  
  if((fd2 = open(argv[2], O_RDWR)) < 0) {
    printf(1, "cmp: cannot open %s for reading\n", argv[2]);
    exit();
  }
  char *buffer2;
  buffer2 = (char*) malloc(1000000 * sizeof(char));
  int buffSize2 = 0;
  while((n = read(fd2, buf, sizeof(buf))) > 0)
  {
    for(i = 0; i < n; i++)
    {
      buffer2[buffSize2] = (char)buf[i];
      buffSize2++;
    }
  }
  if(n < 0) {
    printf(1, "cmp: read error\n");
    exit();
  }
  int lines = 1;
  if(buffSize > buffSize2) {
  	for(i = 0; i < buffSize; i++) {
  	  if(buffer[i] != buffer2[i]) {
  	    printf(1, "%s %s differ: byte %d, line %d\n", argv[1], argv[2], i, lines);
  	    exit();
	  }
	  if(buffer[i] == '\n') lines++;
	}
  }
  printf(1, "_\n");
  exit();
}
