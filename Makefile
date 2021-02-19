CC = gcc
CFLAGS = -Wpedantic -Wall -std=gnu99

myShell: myShell.c 
	$(CC) $(CFLAGS) myShell.c -o myShell