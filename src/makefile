PROJ_NAME = ted

CC = gcc
CFLAGS = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
LDFLAGS = 
LIBS = -lm -lssp
SRCS = main.c lista.c fileManager.c geo.c files.c digraph.c hash.c via.c priorityQueue.c streap.c qry.c

OBJETOS = $(SRCS:.c=.o)

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

lista.o: lista.c lista.h
fileManager.o: fileManager.c fileManager.h
geo.o: geo.c geo.h
files.o: files.c files.h
digraph.o: digraph.c digraph.h
hash.o: hash.c hash.h
via.o: via.c via.h
priorityQueue.o: priorityQueue.c priorityQueue.h
streap.o: streap.c streap.h
qry.o: qry.c qry.h

main.o: main.c lista.h fileManager.h geo.h files.h digraph.h via.h

clean:
	del /Q $(OBJETOS) $(PROJ_NAME).exe 2>nul || true