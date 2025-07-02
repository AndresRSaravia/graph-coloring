CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99 -g
DEPS = JonSnow.h
OBJ = IronThrone.o DaenerysTargaryen.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

IronThrone: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
valgrind:
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./IronThrone
clean:
	rm -f *.o IronThrone
