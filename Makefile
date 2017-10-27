EXEC= pruebas
OBJFILES= testing.o pila.o abb.o pruebas_alumno.o main.o

CC=gcc
CFLAGS= -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion
VFLAGS= --leak-check=full --track-origins=yes --show-reachable=yes

all:$(EXEC)

run: all
	./$(EXEC)

valgrind: all
	valgrind $(VFLAGS) ./$(EXEC)

gdb: all
	gdb ./$(EXEC)

clean:
	rm -f *.o $(EXEC)

$(EXEC): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $(EXEC)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
