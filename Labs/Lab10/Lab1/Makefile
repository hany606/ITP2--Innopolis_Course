# http://www.gnu.org/software/make/manual/make.html#Makefile-Names
CC = gcc
BIN = vehcile
OBJECTS = main.o speed.o

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) main.o speed.o -o $(BIN)

main.o: main.c speed.h
	$(CC) -c main.c

speed.o: speed.h speed.c
	$(CC) -c speed.c


#clean will remove the binary(executable file) and the objects files
#in order to have only the source code without have the building files
clean:
	rm -f $(OBJECTS) $(BIN)
# speed.o: speed.h speed.c
# 	gcc -c speed.c
