all : Exercicio2 PI clean

Exercicio2 : Experimento2.o
	gcc -o Experimento2 Experimento2.o

PI : calculoPI.o
	gcc -o pi calculoPI.o -lm

pi.o : calculoPI.c
	gcc -c calculoPI.o -lm
	
Experimento2.o : Experimento2.c
	gcc -c Experimento2.c

clean :
	rm *.o
	
