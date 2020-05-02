#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[1000000];

void
base64Encoder(char str[], int l)
{
  char key[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  char *enc = (char *) malloc(1000000 * sizeof(char));
  int id, bits = 0, sisa = 0, val = 0, c = 0, temp;
  int i, j, k = 0;
  for(i = 0; i < l; i += 3)
  {
    val = 0, c = 0, bits = 0;
	for(j = i; j < l && j <= i + 2; j++)
	{
	  val <<= 8;
	  val |= str[j];
	  c++;
	}
	bits = c * 8;
	sisa = bits % 3;
	while(bits > 0)
	{
	  if(bits >= 6)
	  {
	    temp = bits - 6;
		id = (val >> temp) & 63;
		bits -= 6;
	  }
	  else
	  {
		temp = 6 - bits;
		id = (val << temp) & 63;
		bits = 0;
	  }
	  enc[k++] = key[id];
	}
  }
  for(i = 1; i <= sisa; i++) enc[k++] = '=';
  enc[k] = '\0';
  printf(1, "%s\n", enc);
}

int
main(int argc, char *argv[])
{
  int fd, i, n;
  if(argc <= 1) {
    printf(1, "base64: need 1 argument!\n");
    exit();
  }
  if((fd = open(argv[1], O_RDONLY)) < 0) {
    printf(1, "base64: cannot open %s for reading\n", argv[1]);
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
  close(fd);
  if(n < 0) {
    printf(1, "base64: read error\n");
    exit();
  }
  base64Encoder(buffer, buffSize);
  exit();
}
