#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int fib(int n){
	int arr[512];
	if (n==0){
		arr[0]=0;
		return 0;
	}
	if (n==1){
		arr[511]=0;
		arr[0]=1;
		return arr[0];
	}
	else{
		return fib(n-1) + fib(n-2);
	}
}

int 
main(void)
{	
	int x;
	x=fib(4);
	printf(1,"x=%d\n",x );
	exit();
}