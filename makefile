all:
main : main.c keyboard.c 
	gcc -o main main.c keyboard.c list.o -v