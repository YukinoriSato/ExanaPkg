TARGET = bmt.fixed

#CC = icc
CC = gcc
CFLAGS = -g -O3 -fopenmp

SRCS = himenoBMTxpa_omp.c

all: ${TARGET}

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} -o $@ ${SRCS}

test: ${SRCS}
	${CC} ${CFLAGS} -DTEST -o $@ ${SRCS}

clean :
	rm -f ${TARGET} test
