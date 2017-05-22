#include "types.h"
#include "defs.h"
#include "param.h"
#include "semaphore.h"
#include "spinlock.h"
#include "proc.h"

struct {
  struct spinlock lock;
  struct semaphore sem[MAXSEM];
} semtable;

//Creates or retun a semaphores descriptor
//Returns semaphore id or eror code
int
semget(int sem_id, int init_value){
  int id;
  struct semaphore s;
  adquire(&semtable.lock);
  if (sem_id < -1 || sem_id == 0) {
    release(&semtable.lock);
    return -4;
  } //Invalid sem_id argument
  
  if (sem_id == -1) { //Get new semaphore
    //Search empty place in process semaphores
    for (id = 0; id < MAXSEMPROC && proc->osem[id]!=0; ++id){
      //nothing, just skip
    }
    if(id==MAXSEMPROC){ //process has no free place
      release(&semtable.lock);
      return(-2);
    }
    //Search empty place in semtable
    for(s = semtable.sem; s < &semtable.sem[MAXSEM]; s++){
      if(!s->in_use){ //Found free sem
        //init semaphore and vinculate to process
        s->in_use = 1;
        s->id = s-semtable.sem+1; //plus 1 to avoid id=0
        s->refcount++;
        s->val = init_value;
        proc->osem[id] = s;
        release(&semtable.lock);
        return(s->id);
      }
    }
    //No free sem in system
    release(&semtable.lock);
    return(-3);
  }
  else{ //Search for sem_id
    for(s = semtable.sem; s < &semtable.sem[MAXSEM]; s++){
      if(s->id == sem_id){ //Found sem
        if(s->in_use){
          //Search empty place in process semaphores
          for (id = 0; id < MAXSEMPROC && proc->osem[id]!=0; ++id){
            //nothing, just skip
          }
          if(id==MAXSEMPROC){ //process has no free place
            release(&semtable.lock);
            return(-2);
          }
          //vinculate semaphore to process
          s->refcount++;
          proc->osem[id] = s;
          release(&semtable.lock);
          return(s->id); //exit on succes
        }
        //Sem_id is not in use
        release(&semtable.lock);
        return(-1);
      }
    }
    //Sem_id not found
    release(&semtable.lock);
    return(-1);
  }
}

//Release semaphore
int
semfree(int sem_id){
  int id;
  struct semaphore s;
  adquire(&semtable.lock);
  //Search for sem_id
  for(s = semtable.sem; s < &semtable.sem[MAXSEM]; s++){
    if(s->id == sem_id){ //Found sem
      if(s->in_use){
        //Search if the process has the semaphore
        for (id = 0; id < MAXSEMPROC && proc->osem[id] != s; ++id){
          //nothing, just skip
        }
        if(id==MAXSEMPROC){ //process is not vinculated to semaphore
          release(&semtable.lock);
          return(-1);
        }
        //decrement ref count and devinculate process
        s->refcount--;
        if (s->refcount==0){
          s->in_use = 0;
        }
        proc->osem[id] = 0;
        release(&semtable.lock);
        return(0); //exit on succes
      }
      //Sem_id is not in use
      release(&semtable.lock);
      return(-1);
    }
  }
  //Sem_id not found
  release(&semtable.lock);
  return(-1);
}

//Decrement semaphore value
int
semdown(int sem_id){
  int id;
  struct semaphore s;
  adquire(&semtable.lock);
  //Search for sem_id
  for(s = semtable.sem; s < &semtable.sem[MAXSEM]; s++){
    if(s->id == sem_id){ //Found sem
      if(s->in_use){
        //Search if the process has the semaphore
        for (id = 0; id < MAXSEMPROC && proc->osem[id] != s; ++id){
          //nothing, just skip
        }
        if(id==MAXSEMPROC){ //process is not vinculated to semaphore
          release(&semtable.lock);
          return(-1);
        }
        //decrement sem val or lock caller process
        while(s->val < 1){
          sleep(&semtable, &semtable.lock);
        }
        s->val--;
        release(&semtable.lock);
        return(0); //exit on sucess
      }
      //Sem_id is not in use
      release(&semtable.lock);
      return(-1);
    }
  }
  //Sem_id not found
  release(&semtable.lock);
  return(-1);
}

//Increment semaphore value
int
semup(int sem_id){
  int id;
  struct semaphore s;
  adquire(&semtable.lock);
  //Search for sem_id
  for(s = semtable.sem; s < &semtable.sem[MAXSEM]; s++){
    if(s->id == sem_id){ //Found sem
      if(s->in_use){
        //Search if the process has the semaphore
        for (id = 0; id < MAXSEMPROC && proc->osem[id] != s; ++id){
          //nothing, just skip
        }
        if(id==MAXSEMPROC){ //process is not vinculated to semaphore
          release(&semtable.lock);
          return(-1);
        }
        //increment sem val and wake up sleeping process
        s->val++;
        wakeup(&semtable);
        release(&semtable.lock);
        return(0); //exit on sucess
      }
      //Sem_id is not in use
      release(&semtable.lock);
      return(-1);
    }
  }
  //Sem_id not found
  release(&semtable.lock);
  return(-1);
}