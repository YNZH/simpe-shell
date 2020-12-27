#include "interpreter.h"

/*
 * interprete shell command without pipe symbol
 * */
int run_shell(int argc,char argv[MAX_PIPE_NUM][MAX_ARGC][MAX_LEN])
{
		int redirect = -1;   //redirect flag
		int stdio_fd[2];     
		char filepath[1024];
		int fd = -1;

		for(int i=0;i<argc;i++)
		{
				if(strcmp(argv[0][i],"<") == 0 && i + 1 < argc)
				{
						argc = i;   //truncate arg length because of redirect singal which is not belong args.
						redirect = 0;
						strcpy(filepath,argv[0][i+1]);
						fd = open(filepath,O_RDONLY);
						if(fd < 0)
						{
								fprintf(stderr,"open %s failed\n",filepath);
								exit(1);
						}
						stdio_fd[0] = dup(STDIN_FILENO);
						dup2(fd,STDIN_FILENO);
						close(fd);
						break;
				}
				if(strcmp(argv[0][i],">") == 0 && i + 1 < argc) 
				{ 
						argc = i;   //truncate arg length because of redirect singal which is not belong args.
						redirect = 1;
						strcpy(filepath,argv[0][i+1]);
						fd = open(filepath,O_WRONLY | O_CREAT,S_IRWXU);
						if(fd < 0)
						{
								fprintf(stderr,"open %s failed\n",filepath);
								exit(1);
						}

						stdio_fd[1] = dup(STDOUT_FILENO);
						dup2(fd,STDOUT_FILENO);
						close(fd);
						break;
				}
		}
		pid_t pid = fork();
		if(pid < 0)
		{
				fprintf(stderr,"fork error");
				exit(-1);
		}
		if(pid > 0) // parent
		{
				wait(NULL);
				if(redirect == 1)
				{
						dup2(stdio_fd[1],STDOUT_FILENO);
						close(stdio_fd[1]);
				}
				if(redirect == 0)
				{
						dup2(stdio_fd[0],STDIN_FILENO);
						close(stdio_fd[0]);
				}
				printf("%s",SHELL_ICON);
		}
		else
		{
				char *  _argv[argc+1];
				for(int i=0;i<argc;i++)
				{
						_argv[i] = argv[0][i];
				}
				_argv[argc] = NULL;
				execv(_argv[0],_argv);
				fprintf(stderr,"exec %s error\n",_argv[0]);
				exit(1);
		}	
		return 0;
}

/*
 * interprete shell command with pipe symbol
 *
 * */
int run_pipe_shell(int pipe_num,int argc_group[MAX_LEN],char argv_group[MAX_PIPE_NUM][MAX_ARGC][MAX_LEN]) 
{
		int fd[pipe_num][2];
		char * _argv_group[MAX_LEN][MAX_LEN];
		for(int i=0;i<pipe_num+1;i++)
		{
				for(int j=0;j<argc_group[i];j++)
				{
						_argv_group[i][j] = argv_group[i][j];
				}
				_argv_group[i][argc_group[i]] = NULL;
		}
		pid_t child[pipe_num+1];
		for(int i=0;i<pipe_num+1;i++)
		{
				if(i < pipe_num)
				{
						pipe(fd[i]);
				}
				pid_t pid = fork();
				if(pid < 0)
				{
						fprintf(stderr,"fork error");
						exit(1);
				}
				child[i] = pid;
				if(pid == 0)
				{
						if(i == 0)
						{
								close(fd[i][0]);
								dup2(fd[i][1],STDOUT_FILENO);
								close(fd[i][1]);

								//the first process maybe include input redirect symbol
								char filepath[MAX_LEN];
								for(int ii=0;ii < argc_group[0];ii++)
								{
										if(strcmp(_argv_group[0][ii],"<") == 0 && ii + 1 < argc_group[0])
										{
												argc_group[0] = ii;   //truncate arg length because of redirect singal which is not belong args.
												//redirect = 0;
												strcpy(filepath,_argv_group[0][ii+1]);
												_argv_group[0][ii] = NULL;
												int redirect_in_fd = open(filepath,O_RDONLY);
												if(redirect_in_fd < 0)
												{
														fprintf(stderr,"open %s failed\n",filepath);
														exit(1);
												}
												//stdio_fd[0] = dup(STDIN_FILENO);
												dup2(redirect_in_fd,STDIN_FILENO);
												close(redirect_in_fd);
												break;
										}
								}

								execv(_argv_group[i][0],_argv_group[i]);
								fprintf(stderr,"execv %s error \n",_argv_group[i][0]);
								exit(1);
						}
						else if(i < pipe_num)
						{
								for(int j=0;j<i;j++)
								{
										close(fd[j][1]);
								}
								dup2(fd[i-1][0],STDIN_FILENO);
								close(fd[i-1][0]);

								close(fd[i][0]);
								dup2(fd[i][1],STDOUT_FILENO);
								close(fd[i][1]);
								execv(_argv_group[i][0],_argv_group[i]);
								fprintf(stderr,"execv %s error \n",_argv_group[i][0]);
								exit(1);
						}
						else
						{
								for(int j=0;j<i;j++)
								{
										close(fd[j][1]);
								}


								//the last process maybe include output redirect symbol
								char filepath[MAX_LEN];
								for(int ii=0;ii < argc_group[i];ii++)
								{
										if(strcmp(_argv_group[i][ii],">") == 0 && ii + 1 < argc_group[i])
										{
												argc_group[i] = ii;   //truncate arg length because of redirect singal which is not belong args.
												//redirect = 0;
												strcpy(filepath,_argv_group[i][ii+1]);
												_argv_group[i][ii] = NULL;
												int redirect_out_fd = open(filepath,O_WRONLY | O_CREAT,S_IRWXU);
												if(redirect_out_fd< 0)
												{
														fprintf(stderr,"open %s failed\n",filepath);
														exit(1);
												}
												//stdio_fd[0] = dup(STDIN_FILENO);
												dup2(redirect_out_fd,STDOUT_FILENO);
												close(redirect_out_fd);
												break;
										}
								}

								dup2(fd[i-1][0],STDIN_FILENO);
								close(fd[i-1][0]);
								execv(_argv_group[i][0],_argv_group[i]);
								fprintf(stderr,"execv %s error \n",_argv_group[i][0]);
								exit(1);
						}
				}
		}
		for(int i=0;i<pipe_num;i++)
		{
				close(fd[i][0]);
				close(fd[i][1]);
		}
		for(int i=0; i< pipe_num+1;i++)
		{
				waitpid(child[i],NULL,0);
		}
		printf("%s",SHELL_ICON);
		return 0;
}
