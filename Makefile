CC = gcc
CFLAGS = -Wall -O2

OBJ = main.o buddy_allocator.o

buddy_allocator: $(OBJ)
	$(CC) -o buddy_allocator $(OBJ)

main.o: main.c buddy_allocator.h
	$(CC) $(CFLAGS) -c main.c

buddy_allocator.o: buddy_allocator.c buddy_allocator.h
	$(CC) $(CFLAGS) -c buddy_allocator.c

clean:
	rm -f *.o buddy_allocator

