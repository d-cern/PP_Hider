CC = gcc
CFLAGS = -Wall -g
OBJFILES = main.o paletteParityHider.o fileHandler.o

all: PP_Hider.exe

clean:
	rm -f *.o PP_Hider.exe


fileHandler.o: fileHandler.c fileHandler.h
	$(CC) $(CFLAGS) -c fileHandler.c

paletteParityHider.o: paletteParityHider.c paletteParityHider.h
	$(CC) $(CFLAGS) -c paletteParityHider.c

main.o: main.c paletteParityHider.h fileHandler.h
	$(CC) $(CFLAGS) -c main.c

PP_Hider.exe: main.o paletteParityHider.o fileHandler.o
	$(CC) $(CFLAGS) $(OBJFILES) -lm -o PP_Hider.exe