CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

OBJS = my_malloc.o my_calloc.o my_free.o my_realloc.o

libmymemory.a: $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o libmymemory.a
