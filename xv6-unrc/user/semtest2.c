#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define MAX 10

int main(int argc, char const *argv[])
{
  int fd,fd2,i,l,fid;
  char path[] = "fsem";
  int sem;

  char buf[512];
  char a[] = "0";
  printf(1,"a = %s\n",a );

  sem = semget(-1,1);
  if (sem<0) exit();
  printf(1,"semget ok\n");
  fd = open(path, O_CREATE | O_RDWR);
  fd2 = open(path, O_RDONLY);
  if (fd<0) exit();
  printf(1,"open ok, fd=%d, fd2=%d\n",fd,fd2);
  
  if ( write(fd, a, sizeof(a)) < 0) exit();
  printf(1,"write ok\n");
  fid = fork();
  if(fid==0) semget(sem,0);
  for(i=0; i < MAX; i++){
    if (fid == 0) printf(1,"child ");
    else          printf(1,"father ");
    if ( semdown(sem) < 0) exit();
    //printf(1,"semdown ok\n" );
    l=read(fd2,buf,sizeof(buf));
    if ( l < 0) exit();
    printf(1,"readed %d bytes :'%s'\n",l,buf );

    write(fd,buf,l);
    if ( semup(sem) < 0) exit();
    //printf(1,"semup ok\n" );
  }
  if(fid==0) exit();
  
  wait();

  if(semfree(sem)<0) exit();
  printf(1,"semfree ok\n");
  exit();
}