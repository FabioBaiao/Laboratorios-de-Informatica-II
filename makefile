CFLAGS=-Wall -Wextra -ansi -pedantic -O2 -g
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
LIBS=

prot: $(OBJS)
	$(CC) $(CFLAGS) -o prot $(OBJS) $(LIBS) 

do: prot
	./prot

limpar: 
	rm prot *.o 

test: prot
	cd testes; bash testar.sh ../prot