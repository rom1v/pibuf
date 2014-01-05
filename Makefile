OUT = pibuf
OBJ = main.o cbuf.o naldetector.o
CFLAGS = -g -Wall

pibuf : $(OBJ)
	cc $(CFLAGS) -o pibuf $(OBJ)

$(OBJ) : cbuf.h naldetector.h

clean :
	rm -f $(OBJ) pibuf
