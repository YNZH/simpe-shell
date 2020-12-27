#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHELL_ICON "# :"
#define MAX_LEN 128
#define MAX_PIPE_NUM 8
#define MAX_ARGC 8 

int run_shell(int argc,char argv[MAX_PIPE_NUM][MAX_ARGC][MAX_LEN]);
int run_pipe_shell(int pipe_num,int argc_group[MAX_LEN],char argv_group[MAX_PIPE_NUM][MAX_ARGC][MAX_LEN]);
#endif
