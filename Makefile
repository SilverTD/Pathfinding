CC = g++

INCLUDE = -I ./include

CFLAGSO = -W ${INCLUDE}

LIBS = -lSDL2main -lSDL2

CFLAGSB = ${LIBS} -o main

build:
	${CC} ./src/*.cpp ${CFLAGSO} ${CFLAGSB}
