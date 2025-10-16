PROJ_NAME = ted

CC = gcc
CFLAGS = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
LDFLAGS = 
LIBS = -lm -lssp
SRCS = main.c lista.c fileManager.c geo.c svg.c # digraph.c hash.c priority_queue.c fila.c streap.c utils.c via.c geo.c svg.c qry.c

OBJETOS = $(SRCS:.c=.o)

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

lista.o: lista.c lista.h
filemanager.o: fileManager.c fileManager.h
geo.o: geo.c geo.h
svg.o: svg.c svg.h
# fila.o: fila.c fila.h
# utils.o: utils.c utils.h

# hash.o: hash.c hash.h utils.h
# priority_queue.o: priority_queue.c priority_queue.h
# streap.o: streap.c streap.h fila.h lista.h

# digraph.o: digraph.c digraph.h lista.h hash.h priority_queue.h streap.h utils.h
# via.o: via.c via.h digraph.h utils.h
# geo.o: geo.c geo.h lista.h
# svg.o: svg.c svg.h digraph.h lista.h

main.o: main.c lista.h fileManager.h geo.h svg.h # utils.h via.h digraph.h lista.h geo.h svg.h qry.h

clean:
	rm -f $(OBJETOS) $(PROJ_NAME)