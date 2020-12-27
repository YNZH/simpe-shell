#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
// simple word count shell
int main(int argc,char * argv[])
{
		int word_count = 0;
		int line_count = 0;
		if(argc  == 1)
		{
				char ch;
				while((ch = getc(stdin)) != EOF)
				{
						if(ch == 0x0A)
								line_count++;
				}
				printf("%d\n",line_count);
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
				while((ch = getc(fd)) != EOF)
				{
						if(ch == 0x0A)
								line_count++;
				}
				printf("%d\n",line_count);
				fclose(fd);
		}
		return 0;
}
