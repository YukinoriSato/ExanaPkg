OBJS = himenoBMTxps.o

CC = gcc
RM = /bin/rm
PROG = bmt

CFLAGS = -O3 -DSMALL
LDFLAGS = ${CFLAGS}

all : ${PROG}

${PROG} : ${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS}

.c.o :
	${CC} -c ${CFLAGS} $<

clean :
	${RM} -f ${PROG} ${OBJS} core

