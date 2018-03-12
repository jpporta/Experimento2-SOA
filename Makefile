all : main clean

main : Experimento2.o
	gcc -Wall Experimento2.o -o main
	
Experimento2.o : Experimento2.c
	gcc -c Experimento2.c

clean :
	rm *.o
	