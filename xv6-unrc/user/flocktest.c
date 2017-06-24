#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

void
locktest(int fd){
  int i,n;
  char buf[512];
  for (i = 0; i < 100; ++i)
  {
    
    flock(fd);
    //printf(1,"pid:%d enter critic zone\n",getpid());
    if ( fseek(fd, 0) < 0) exit();
    read(fd, buf, sizeof(buf));
    //printf(1,"pid:%d readed:%d bytes\n",getpid(),l);
    n = atoi(buf);

    if ( fseek(fd, 0) < 0) exit();
    printf(fd, "%d",++n);
    if ( fseek(fd, 0) < 0) exit();
    //printf(1,"pid:%d exit critic zone\n\n",getpid());  
    funlock(fd);
  }
}

int main(int argc, char const *argv[])
{
  int fd,pid,count;
  char buf[512];
  char *path; 
  if(argc>1) path=(char*)argv[1];
  else path="count";
  fd = open(path, O_CREATE | O_RDWR);
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

  if ( fseek(fd, 0) < 0) exit();  
  read(fd, buf, sizeof(buf));
  count = atoi(buf);
  printf(1,"count = %d\n", count);
  close(fd);
  exit();
}