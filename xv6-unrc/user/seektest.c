#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define MAX 10

int main(int argc, char const *argv[])
{
  int fd,n;
  char path[] = "tmpfile";

  char buf[512];
  char a[] = "hello ";
  char b[] = "       world.";
  fd = open(path, O_CREATE | O_RDWR);

  if ( write(fd, b, sizeof(b)) < 0) exit(); //write '     world.'

  if ( fseek(fd, 0) < 0) exit();

  if ( write(fd, a, sizeof(a)) < 0) exit(); //write 'hello '

  printf(1,"message :\n");
  
  if ( fseek(fd, 0) < 0) exit();
  while((n = read(fd, buf, sizeof(buf))) > 0)
    write(1, buf, n);
  write(1,"\n",1);
  exit();
}