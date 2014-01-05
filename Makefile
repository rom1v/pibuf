OUT = pibuf
OBJ = main.o cbuf.o
CFLAGS = -g -Wall

pibuf : $(OBJ)
	cc $(CFLAGS) -o pibuf $(OBJ)

cbuf.o main.o : cbuf.h

clean :
	rm -f $(OBJ) pibuf
