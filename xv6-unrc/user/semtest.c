#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX 1000

int testget(int *sem){
  int sem2,sem3;
  *sem = semget(-1,1);
  printf(1,"sem_id1 is: %d\n",*sem );
  if (*sem<0) return *sem;
  sem2 = semget(-1,1);
  printf(1,"sem_id2 is: %d\n",sem2 );
  if (sem2<0) return sem2;
  semget(-1,1);
  semget(-1,1);
  // semget(-1,1); causes error (exceded max num of semgets (MAXSEMPROC))
  sem3 = semget(-1,1);
  printf(1,"sem_id3 is: %d\n",sem3 );
  if (sem3<0) return sem3;

  printf(1,"semget ok\n");
  return 0;
}

int main(int argc, char const *argv[])
{
  int n;
  int i;
  int sem;
  if(testget(&sem)) exit();

  //Fork
  if (fork()==0){
    //child
    for(i=0; i < MAX; i++){
      n++;
    }
    exit();
  }else{
    //father
    for(i=0; i < MAX; i++){
      n++;
    }
    wait();
  }
  printf(1,"n= %d\n",n);
  if(semfree(sem)<0) exit();
  printf(1,"semfree ok\n");
  exit();
}