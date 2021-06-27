include ../Make.defines

PROGS =	1-naglecli 1-nagleserv 2-pthreads 3-tcpcli01 3-tcpserv01

all:	${PROGS}

1-naglecli:		1-naglecli.o
		${CC} ${CFLAGS} -o $@ 1-naglecli.o ${LIBS}

1-nagleserv:	1-nagleserv.o
		${CC} ${CFLAGS} -o $@ 1-nagleserv.o ${LIBS}

2-pthreads:		2-pthreads.o
		${CC} ${CFLAGS} -o $@ 2-pthreads.o ${LIBS}
		
3-tcpcli01:		3-tcpcli01.o
		${CC} ${CFLAGS} -o $@ 3-tcpcli01.o ${LIBS}

3-tcpserv01:	3-tcpserv01.o
		${CC} ${CFLAGS} -o $@ 3-tcpserv01.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
