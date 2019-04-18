CFLAGS = -Wall -pedantic

all: main.o serverSetup.o waitAndProcess.o assign2

clean: 
	rm *.o assign2

serverSetup.o: serverSetup.c
	gcc -c -g $(CFLAGS) serverSetup.c -o serverSetup.o

main.o: main.c
	gcc -c -g $(CFLAGS) main.c -o main.o

waitAndProcess.o: waitAndProcess.c
	gcc -c -g $(CFLAGS) waitAndProcess.c -o waitAndProcess.o

assign2: main.o serverSetup.o waitAndProcess.o
	gcc $(CFLAGS) main.o serverSetup.o waitAndProcess.o -o assign2
