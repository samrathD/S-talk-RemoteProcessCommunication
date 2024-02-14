all:
main : main.c keyboard.c 
	gcc -o main main.c keyboard.c receiveProcess.c list.o