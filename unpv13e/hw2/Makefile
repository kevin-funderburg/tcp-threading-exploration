include ../Make.defines

PROGS =	naglecli nagleserv

all:	${PROGS}

naglecli:	naglecli.o
		${CC} ${CFLAGS} -o $@ naglecli.o ${LIBS}

nagleserv:	nagleserv.o
		${CC} ${CFLAGS} -o $@ nagleserv.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
