a.out : stack.o queue.o calc.o main.o
	gcc -o a.out stack.o queue.o calc.o main.o

stack.o : stack.c
	gcc -c -o stack.o stack.c

queue.o : queue.c
	gcc -c -o queue.o queue.c

calc.o : calc.c
	gcc -c -o calc.o calc.c

main.o : main.c
	gcc -c -o main.o main.c

clean:
	rm *.o a.out
