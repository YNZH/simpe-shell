#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
// simple exit shell
int main(int argc,char * argv[])
{
		if(argc  == 1)
		{
				pid_t ppid = getppid();
		}
		else
		{
				fprintf(stderr,"usage : exit (without args)\n");
		}
		return 0;
}
