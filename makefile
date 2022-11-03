CFLAGS = -g -Wall

dict3: dict3.o list.o data.o tree.o
	gcc -Wall -g -o dict3 dict3.o list.o data.o tree.o

dict3.o: dict3.c list.h data.h tree.h
	gcc -Wall -o dict3.o dict3.c -g -c

dict4: dict4.o list.o data.o tree.o
	gcc -Wall -g -o dict4 dict4.o list.o data.o tree.o

dict4.o: dict4.c list.h data.h tree.h
	gcc -Wall -o dict4.o dict4.c -g -c