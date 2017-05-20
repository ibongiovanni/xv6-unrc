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
  if (sem_id < -1 || sem_id == 0) return -4; //Invalid sem_id argument
  if (sem_id == -1) { //Get new semaphore
    //Search empty place in process semaphores

    //Search empty place in semtable
    
  }
  else{

  }
}

//Release semaphore
int
semfree(int sem_id){

}

//Decrement semaphore value
int
semdown(int sem_id){
  
}

//Increment semaphore value
int
semup(int sem_id){
  
}