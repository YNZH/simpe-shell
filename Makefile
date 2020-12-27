BIN=bin

main: smpsh cat ls wc
	
smpsh: src/interpreter.c src/simple_shell.c src/interpreter.h 
	mkdir ${BIN}
	gcc src/interpreter.c src/simple_shell.c -o ${BIN}/smpsh
cat: src/cat.c
	gcc src/cat.c -o ${BIN}/cat
ls: src/ls.c
	gcc src/ls.c -o ${BIN}/ls
wc: src/wc.c
	gcc src/wc.c -o ${BIN}/wc

.PHONY: clean
clean:
	rm -R ${BIN}
