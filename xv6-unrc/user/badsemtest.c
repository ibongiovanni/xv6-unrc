#include "types.h"
#include "user.h"

#define LIMIT 1000
int
main(void)
{	

    int i=0, semid,mutex;

    /*checking for non process sema access*/
    int chk = semup(20);
    if(chk<0)
        printf(1,"Testing non adquired semaphore (up): OK!\n");
    else
        printf(1,"Testing non adquired semaphore (up): FAILED.\n");
    
    chk = semdown(20);
    if(chk<0)
        printf(1,"Testing non adquired semaphore (down): OK!\n");
    else
        printf(1,"Testing non adquired semaphore (down): FAILED.\n");

    /*checking for free*/
    chk=semfree(8);
    if(chk<0)
        printf(1,"Testing semfree: OK!.\n");
    else
        printf(1,"Testing semfree: FAILED!.\n");
    
    /*checking for getting all the resources*/
    int countsem=0,it;
    chk=0;
    for(it=0 ; it<LIMIT && chk>=0; it++,countsem++){
        chk = semget(-1,4);   
        printf(1,"semget: %d\n",chk);
             
    }    
    printf(1,"Testing get all semaphores availables (%d availables?).\n",countsem-1);
    
    /*checking for free: sem 0 was adquired*/
    chk=semfree(0);
    if(chk==0)
        printf(1,"Testing semfree sem 0: OK!.\n");
    else
        printf(1,"Testing semfree sem 0: FAILED!.\n");

    /*checking for free: but not twice!*/
    chk=semfree(0);
    if(chk<0)
        printf(1,"Testing semfree sem 0 (second time): OK!.\n");
    else
        printf(1,"Testing semfree sem 0 (second time): FAILED!.\n");

    /*cheching binary semaphore locking*/


   /*cheching n-ary semaphore*/
   semfree(1); //release one semaphore
   mutex=semget(-1,1); //sinchronize console access
   semid=semget(-1,2);
   if(fork()==0){
     semget(semid,-1);        

     for(i=0;i<10;i++){
       semdown(mutex);
       printf(1,"1st child trying... \n");
       semup(mutex);
       
       semdown(semid);
       
       semdown(mutex);
       printf(1,"1st child in critical region \n");
       semup(mutex);

       semup(semid);
     }
     exit();
   }
   /*second child*/
   if(fork()==0){
     semget(semid,-1);

     for(i=0;i<10;i++){
      semdown(mutex);
      printf(1,"2nd child trying... \n");
      semup(mutex);

      semdown(semid);

      semdown(mutex);
      printf(1,"2nd child in critical region \n");
      semup(mutex);

      semup(semid);
     }   
     exit();
   }
   /*Parent code*/
   for(i=0;i<10;i++){
    semdown(mutex);
    printf(1,"Parent trying... \n");
    semup(mutex);

    semdown(semid);
 
    semdown(mutex);
    printf(1,"Parent in critical region \n");
    semup(mutex);

    semup(semid);
   }
   wait();
   wait();
   exit();

}
