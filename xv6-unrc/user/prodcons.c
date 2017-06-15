//Implementation of Producer-Consumer problem with semaphores

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define BUFFER_SIZE 10
#define PRODUCERS 10
#define CONSUMERS 10

int main(int argc, char const *argv[])
{
  int items;  // items produced
  int space;  // space remaining

  int fid,p,c,pid;

  //Get new semaphores
  if ((items = semget(-1,0))  < 0) exit();
  if ((space = semget(-1,BUFFER_SIZE))  < 0) exit();

  fid = fork();
  /*
  if (fid == 0){
    //Copy semaphores from father
    if (semget(items,0)  < 0) exit();
    if (semget(space,BUFFER_SIZE)  < 0) exit();
  }
  */
  pid = getpid();

  if (fid > 0){
    for (c = CONSUMERS; c > 0; c--){
      if ( semdown(items) < 0) exit();
      printf(1,"pid:%d CONSUMER\n",pid);
      if ( semup(space) < 0) exit();
    }
  }
  else{
    for (p = PRODUCERS; p > 0; p--){
      if ( semdown(space) < 0) exit();
      printf(1,"pid:%d PRODUCER\n",pid );
      sleep(50);
      if ( semup(items) < 0) exit();
    } 
  }
  
  if(fid==0) exit();
  
  wait();

  if(semfree(items)<0) exit();
  if(semfree(space)<0) exit();
  exit();
}