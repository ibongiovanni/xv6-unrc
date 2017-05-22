
struct semaphore {
	uint id; //semaphore id : [1..MAXSEM]
	int val; //semaphore value
	int refcount; //n of process referencing to the semaphore
	char in_use; //sem state

}; 