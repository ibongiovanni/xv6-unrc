#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define MAX 10

int main(int argc, char const *argv[])
{
  char *oldpath,*newpath;

  if (argc < 3)
  {
    printf(2,"usage: 'rename oldpath newpath'\n" );
    exit();
  }
  oldpath=(char*)argv[1];
  newpath=(char*)argv[2];

  if(open(oldpath, O_RDONLY) < 0){
    printf(2,"error: %s does not exist.\n",oldpath );
    exit();
  }
  if(link(oldpath,newpath) < 0){
    printf(2,"error: cannot link to %s.\n",newpath );
    exit();
  }
  unlink(oldpath);
  exit();
}