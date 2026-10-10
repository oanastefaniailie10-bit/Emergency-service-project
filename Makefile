CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: build clean

build: 
	 $(CC) $(CFLAGS) tema1.c -o tema1
run:
	./tema1
clean:
	rm tema1
