OUT = pibuf
OBJ = main.o cbuf.o naldetector.o
CFLAGS = -g -Wall -O3

pibuf : $(OBJ)
	cc $(CFLAGS) -o pibuf $(OBJ)

$(OBJ) : cbuf.h naldetector.h

clean :
	rm -f $(OBJ) pibuf
