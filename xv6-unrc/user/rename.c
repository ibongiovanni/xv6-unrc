#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int main(int argc, char const *argv[])
{
  char *oldpath,*newpath;
  int ec;

  if (argc != 3)
  {
    printf(2,"usage: 'rename oldpath newpath'\n" );
    exit();
  }
  oldpath=(char*)argv[1];
  newpath=(char*)argv[2];

  ec=rename(oldpath, newpath);
  if( ec < 0)
  {
    printf(2,"rename: error");
    if(ec==-1) printf(2,": '%s' does not exist",oldpath);
    if(ec==-2) printf(2,": cannot rename '%s' to '%s'",oldpath,newpath);
    printf(2,".\n");
    exit();
  }
  exit();
}