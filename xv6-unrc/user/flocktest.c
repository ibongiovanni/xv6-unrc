#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

void
locktest(int fd){
  int i,n,l;
  char buf[512];
  for (i = 0; i < 10; ++i)
  {
    
    flock(fd);

    if ( fseek(fd, 0) < 0) exit();
    l=read(fd, buf, sizeof(buf));
    printf(1,"pid:%d readed:%d bytes\n",getpid(),l);
    n = atoi(buf);

    if ( fseek(fd, 0) < 0) exit();
    printf(fd, "%d",++n);
    if ( fseek(fd, 0) < 0) exit();
    funlock(fd);
  }
}

int main(int argc, char const *argv[])
{
  int fd,pid;
  fd = open("count", O_CREATE | O_RDWR);
  printf(fd, "%d",0);
  pid=0;
  /*while(i<3 && pid==0){
    pid= fork();
    i++;
    printf(1,"fork in pid:%d\n",getpid() );
  }*/
  pid=fork();
  locktest(fd);
  if(pid==0) exit();
  wait();
  close(fd);
  exit();
}