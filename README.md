# simpe-shell
this is a simple shell for Linux
##Build:
	make

##Clean:
	make clean

##Usage:
	cd ./bin
	./smpsh

##Exmaple:
notes: $: is present system shell symbol,
	   # : is simpe-shell symbol.
```bash
$:ls
Makefile  README.md  bin  src
$:cd ./bin/
$:ls
cat  ls  smpsh  wc

$:./smpsh						# begin simple-shell
# :ls							# "$:" is changed as "# :" ,then run custom ls command
   16984 ./cat
   21888 ./smpsh
   16976 ./wc
   17248 ./ls
   4096 .
# :ls | wc						# test pipe 
5								# line number of output of 'ls' just is 5, that's ok.
# :ls | wc | wc                 # test multi pipes
1								# line number of the second pipe's output just is 1. that's ok.
# :ls > out.txt                 # test output redirect form stdout to out.txt.
# :cat out.txt                  # view out.txt 
0    16984 ./cat
1    21888 ./smpsh
2        0 ./out.txt
3    16976 ./wc
4    17248 ./ls
5     4096 .                    # redirect is ok.
6 # :exit						# exit
$:							    # back to system shell	
```

##Features:
1. support some redirct symbols: {'>', '<','|'}
2. support run custom simple shell commands whose is implemented basically by me
   egs:ls, wc, cat, exit, quit.

##How to add new command:
for example, add a wordcount(wc) to simple-shell.
1. create a wordcount.c file
2. complete wordcount.c as your interests.
3. move wordcount.c to ./src dir.
4. edit Makefile
5. that's all



