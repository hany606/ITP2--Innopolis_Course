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

clean:
	rm -f $(OBJECTS) $(BIN)
# speed.o: speed.h speed.c
# 	gcc -c speed.c
