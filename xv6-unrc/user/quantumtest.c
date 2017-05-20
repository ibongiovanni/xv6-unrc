#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int 
main(void)
{	
	set_priority(3);
	procstat();
	int i=50000;
	int p=1;
	while(i>0){
		if (i%2==0) p+=i;
		else p-=i;
		i--;
	}
	printf(1,"p = %d\n",p );
	exit();
}