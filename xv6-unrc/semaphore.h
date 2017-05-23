
struct semaphore {
  uint id; //semaphore id : [0..MAXSEM-1]
  int val; //semaphore value
  int refcount; //n of process referencing to the semaphore

}; 