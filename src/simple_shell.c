#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "interpreter.h"


char argv_group[MAX_PIPE_NUM][MAX_ARGC][MAX_LEN];
int argc_group[MAX_PIPE_NUM];
int pipe_num = 0;


int get_command(char * command);
int get_argv(char * command);
int check_exit();
int main()
{
		char command[MAX_LEN];
		printf("%s",SHELL_ICON);
		int len = 0;
		int fd = -1;
		while( (len = get_command(command)) > 0 )
		{
				get_argv(command);
				check_exit();
#ifdef DEBUG	
				for(int j = 0; j<pipe_num+1;j++)
				{
						for(int k =0 ;k<argc_group[j];k++)
						{
								printf("%s\n",argv_group[j][k]);
						}
						printf("================\n");
				}
#endif

				if (pipe_num == 0)
				{
						run_shell(argc_group[0],argv_group);
				}
				if(pipe_num > 0)
				{
						run_pipe_shell(pipe_num,argc_group,argv_group);
				}
		}

		return 0;
}


int get_argv(char * command)
{
		char * cmd = command;
		char ch = * cmd;
		int idx = 0;
		while(ch)
		{
				while(ch && ch == 0x20) // space
				{
						cmd++;
						ch = *cmd;
				}
				if (!ch) break;
				if(ch == '|')
				{
						pipe_num++;
						cmd++;
						ch = *cmd;
						continue;
				}
				while(ch && ch != 0x20)
				{
						argv_group[pipe_num][argc_group[pipe_num]][idx++] = ch;
						cmd++;
						ch = * cmd;
				}
				argv_group[pipe_num][argc_group[pipe_num]][idx] = 0;
				argc_group[pipe_num]++;
				idx = 0;
		}
		return pipe_num;
}

int get_command(char * command)
{
		memset(argv_group,0,sizeof(argv_group));
		memset(argc_group,0,sizeof(argc_group));
		pipe_num = 0;
		if(command == NULL)
		{
				fprintf(stderr,"command must not be NULL");
				return -1;
		}
		char ch;
		int len = 0;
		while((ch = getchar()) != 0x0A && len < MAX_LEN)
		{
				*(command + len) = ch;
				len++;
		} 
		if(ch == 0x0A)
		{
				*(command + len) = 0;
				return len+1;
		}
		if(len == MAX_LEN)
		{
				fprintf(stderr,"command length greater than %d",MAX_LEN);
				return -1;

		}
		return len;
}

int check_exit()
{
		if(strcmp(argv_group[0][0],"exit") == 0 || strcmp(argv_group[0][0],"quit") == 0)
		{
				exit(0);
		}
		else
		{
				return 0;
		}
}
