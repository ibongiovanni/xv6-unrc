#include "types.h"
#include "defs.h"
#include "param.h"
#include "semaphore.h"
#include "spinlock.h"
#include "mmu.h" //It is necessary to include it to include proc.h?
#include "proc.h"

//atomice release and return code process
#define relandret(c) \
  release(&semtable.lock); \
  return c

//Check that sem_id is inside the proper margins
#define checksemid(sem_id,c) \
  if (sem_id < -1 || sem_id >= MAXSEM) \
    return c
  

struct {
  struct spinlock lock;
  struct semaphore sem[MAXSEM];
} semtable;

//Search in proc->osems if any points to same as 's'
//return arrays position if found, otherwise -1 
static int
searchosems(struct semaphore *s){
  int id;
  for (id = 0; id < MAXSEMPROC && proc->osem[id] != s; ++id){
    //nothing, just skip
  }
  if(id==MAXSEMPROC){ //s not found in osems
    return -1;
  }
  return id;
}


//Creates or return a semaphores descriptor
//Returns semaphore id or error code
int
semget(int sem_id, int init_value){
  int id;
  struct semaphore *s;
  
  checksemid(sem_id,-4);
  acquire(&semtable.lock);
  if (sem_id == -1) { //Get new semaphore
    //Search empty place in process semaphores
    id = searchosems(0);
    if (id<0){  //process has no free place
      relandret(-2);
    }
    //Search empty place in semtable
    for(s = semtable.sem; s < &semtable.sem[MAXSEM]; s++){
      if(s->refcount==0){ //Found free sem
        //init semaphore and vinculate to process
        s->id = s-semtable.sem; //sem id is array position
        s->refcount++;
        s->val = init_value;
        proc->osem[id] = s;
        relandret(s->id);
      }
    }
    //No free sem in system
    relandret(-3);
  }
  else{ //Get semaphore in use
    s = &semtable.sem[sem_id];
    if (s->refcount==0){ //sem_id is not in use
      relandret(-1);
    }
    id = searchosems(0);
    if (id<0){  //process has no free place
      relandret(-2);
    }
    //vinculate semaphore to process
    s->refcount++;
    proc->osem[id] = s;
    relandret(s->id); //exit on succes
  }
}

//Release semaphore
int
semfree(int sem_id){
  int id;
  struct semaphore *s;

  checksemid(sem_id,-1);
  acquire(&semtable.lock);
  s = &semtable.sem[sem_id];
  if (s->refcount==0){ //sem_id is not in use
    relandret(-1);
  }
  id = searchosems(s);
  if (id<0){  //process is not vinculated to semaphore
    relandret(-1);
  }
  //decrement ref count and devinculate process
  s->refcount--;
  proc->osem[id] = 0;
  relandret(0); //exit on succes
}

//Decrement semaphore value
int
semdown(int sem_id){
  int id;
  struct semaphore *s;

  checksemid(sem_id,-1);
  acquire(&semtable.lock);
  s = &semtable.sem[sem_id];
  if (s->refcount==0){ //sem_id is not in use
    relandret(-1);
  }
  id = searchosems(s);
  if (id<0){  //process is not vinculated to semaphore
   relandret(-1);
  }
  //decrement sem val or lock caller process
  while(s->val < 1){
    sleep(&semtable, &semtable.lock);
  }
  s->val--;
  relandret(0); //exit on sucess
}

//Increment semaphore value
int
semup(int sem_id){
  int id;
  struct semaphore *s;

  checksemid(sem_id,-1);
  acquire(&semtable.lock);
  s = &semtable.sem[sem_id];
  if (s->refcount==0){ //sem_id is not in use
    relandret(-1);
  }
  id = searchosems(s);
  if (id<0){  //process is not vinculated to semaphore
    relandret(-1);
  }
  //increment sem val and wake up sleeping process
  s->val++;
  wakeup(&semtable);
  relandret(0); //exit on sucess
}