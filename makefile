all:
main : main.c keyboard.c receiveProcess.c
	gcc -o main main.c keyboard.c receiveProcess.c sendProcess.c print.c list.o