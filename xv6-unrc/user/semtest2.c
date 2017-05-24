#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define MAX 10

int main(int argc, char const *argv[])
{
  int fd,i,n,fid;
  char path[] = "fsem";
  int sem;
  char r[2];
  char a[] = "cc";
  printf(1,"a = %s\n",a );
  memset(r, 0, sizeof(r));

  sem = semget(-1,1);
  if (sem<0) exit();
  printf(1,"semget ok\n");
  fd = open(path, O_CREATE | O_RDWR);
  if (fd<0) exit();
  printf(1,"open ok, fd=%d\n",fd);
  n=0;
  if ( write(fd, a, sizeof(a)) < 0) exit();
  printf(1,"write ok\n");

  fid = fork();
  for(i=0; i < MAX; i++){
      if ( read(fd,r,sizeof(r)) < 0) exit();
      printf(1,"readed '%s'\n",r );
      n = atoi(r);
      n++;
      write(fd,&n,sizeof(n));
  }
  if(fid==0) exit();
  
  wait();
  printf(1,"n= %d\n",n);
  if(semfree(sem)<0) exit();
  printf(1,"semfree ok\n");
  exit();
}