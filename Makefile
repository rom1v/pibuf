OUT = pibuf
OBJ = main.o cbuf.o
CFLAGS = -g

pibuf : $(OBJ)
	cc $(CFLAGS) -o pibuf $(OBJ)

cbuf.o main.o : cbuf.h

clean :
	rm -f $(OBJ) pibuf
