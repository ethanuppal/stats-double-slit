# Copyright (C) 2023 Ethan Uppal. All rights reserved.

CFLAGS   += -Iinclude -std=c99 -pedantic
WARNINGS += -Wall -Wextra
DEBUG    := -g
RELEASE  := -O3
SRC      := $(wildcard src/*.c)
OBJ      := ${SRC:.c=.o}
PRG      := main
CFLAGS   += ${WARNINGS} # ${DEBUG} ${RELEASE}

run: ${PRG}
	./${PRG} > out.txt

${PRG}: ${OBJ} main.c
	${CC} ${CFLAGS} $^ -o ${PRG}

.c.o:
	${CC} ${CFLAGS} $< -c -o ${<:.c=.o}

clean:
	rm -rf ${PRG} ${OBJ}

repair:
	pnmtopnm data/data.pgm > data.pgm && mv data.pgm data
