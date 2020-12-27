#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
// simple cat shell
int main(int argc,char * argv[])
{
		if(argc  == 1)
		{
				//fprintf(stderr,"usage: cat [file1 ...] \n");
				//exit(1);
				char ch;
				while((ch = getc(stdin)) != EOF)
				{
						printf("%c",ch);
				}
				return 0;
		}
		for(int i=1;i<argc;i++)
		{
				FILE* fd = fopen(argv[i],"r");
				if(fd == NULL)
				{
						fprintf(stderr,"open %s failed\n",argv[i]);
						exit(1);
				}
				char ch;
				int linenum = 0;
				printf("%d ",linenum++);
				while((ch = getc(fd)) != EOF)
				{
						printf("%c",ch);
						if(ch == 0x0A)
								printf("%d ",linenum++);
				}
		}
		return 0;
}
