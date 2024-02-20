all: main

main : main.c keyboard.c receiveProcess.c
	gcc -o main main.c keyboard.c receiveProcess.c sendProcess.c print.c threadcancel.c list.o -o s-talk -lpthread

clean:
	rm -f s-talk