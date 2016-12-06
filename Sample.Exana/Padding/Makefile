TARGET = bmt

#CC = icc
CC = gcc
#CC = icc -no-vec
CFLAGS = -g -std=c99 -O3 -Wall -Wextra -fopenmp
#CFLAGS = -g -std=c99 -O3 -Wall -Wextra 

SRCS = himenoBMTxpa_omp.c

all: ${TARGET}

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} -o $@ ${SRCS}

test: ${SRCS}
	${CC} ${CFLAGS} -DTEST -o $@ ${SRCS}

clean :
	rm -f ${TARGET} test
