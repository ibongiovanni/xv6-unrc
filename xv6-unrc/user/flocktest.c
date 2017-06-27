/*
  User Program to test flock & funlock syscalls.
  Create CHILDS processes that work concurrently
  over a file (FILE) reading an int value and writing
  its succesor CICLES times.
  Use:
   > flocktest [FILE]
   FILE: Output file
*/


#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define CICLES 10
#define CHILDS 10

void
locktest(int fd){
  int i,n;
  char buf[512];
  for (i = 0; i < CICLES; ++i)
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
  int fd,pid,count,kid;
  char buf[512];
  char *path; 
  
  //Set path
  if(argc>1) path=(char*)argv[1];
  else path="count";

  //Open file
  fd = open(path, O_CREATE | O_RDWR);
  printf(fd, "%d",0); //initialize with 0
  
  //Create multiple processes
  for (kid = 0; kid < CHILDS; ++kid) {
    pid = fork();
    if(pid < 0){
        exit();
    }
    else if (pid > 0){
        /* Parent process */
    }
    else{
        /* Child process */
        //Run test simultaneously
        locktest(fd);
        exit();
    }
  }

  for (kid = 0; kid < CHILDS; ++kid) {
      wait(); // kids could be ready in any order
  }

  //Show Result
  if ( fseek(fd, 0) < 0) exit();  
  read(fd, buf, sizeof(buf));
  count = atoi(buf);
  printf(1,"count = %d\n", count);
  
  //Close file
  close(fd);
  exit();
}