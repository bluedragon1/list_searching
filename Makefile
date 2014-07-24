CC = gcc
#if you use very old gcc, remove '-march=native'
CFLAGS = -Wall -O3 -pthread
#EXECUTABLE = pthread list

pthread: pthread.c list.c list.h
#all: $(EXECUTABLE)

#clean:
#	rm -f $(EXECUTABLE)
