/*
  User Program to test flock & funlock syscalls.
  Use:
   > flocktest [FILE]
   FILE: Output file
*/


#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define MAX 10

void
locktest(int fd){
  int i,n;
  char buf[512];
  for (i = 0; i < MAX; ++i)
  {
    
    flock(fd);
    if ( fseek(fd, 0) < 0) exit();
    read(fd, buf, sizeof(buf));
    n = atoi(buf);

    if ( fseek(fd, 0) < 0) exit();
    printf(fd, "%d",++n);
    if ( fseek(fd, 0) < 0) exit(); 
    funlock(fd);
  }
}

int main(int argc, char const *argv[])
{
  int fd,pid,count;
  char buf[512];
  char *path; 
  
  //Set path
  if(argc>1) path=(char*)argv[1];
  else path="count";

  //Open file
  fd = open(path, O_CREATE | O_RDWR);
  printf(fd, "%d",0); //initialize with 0
  
  //Create multiple processes
  pid=fork();
  
  //Run test simultaneously
  locktest(fd);

  //Close processes
  if(pid==0) exit();
  wait();

  //Show Result
  if ( fseek(fd, 0) < 0) exit();  
  read(fd, buf, sizeof(buf));
  count = atoi(buf);
  printf(1,"count = %d\n", count);
  
  //Close file
  close(fd);
  exit();
}