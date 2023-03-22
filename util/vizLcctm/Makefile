OBJS = generateDot.o print.o file.o main.o

CXX = g++ -std=c++0x
RM = /bin/rm
PROG = generateDot

#CXXFLAGS = -O2 -g -I${EXANA_DIR}/util/graphviz/include/graphviz -L${EXANA_DIR}/util/graphviz/lib $(shell pkg-config libgvc --cflags --libs)  
CXXFLAGS = -O2 -g 
#-lstdc++
#LDFLAGS = ${CXXFLAGS} -lgvc -I${EXANA_DIR}/util/graphviz/include/graphviz -L${EXANA_DIR}/util/graphviz/lib
#LDFLAGS = ${CXXFLAGS} -lgvc -I/usr/include/graphviz -L/usr/lib64/graphviz
LDFLAGS = ${CXXFLAGS} -lgvc -lcgraph -lcdt -I/usr/include/graphviz
#LDFLAGS = ${CXXFLAGS} -lgvc -lgraph -lcdt
#-lgraph -lcdt
all : ${PROG}

${PROG} : ${OBJS}
	${CXX} -o $@ ${OBJS} ${LDFLAGS}
#	${CXX} -o $@ $@.o ${LDFLAGS}

.cpp.o :
	${CXX} -c ${CFLAGS} ${LDFLAGS} $<

clean :
	${RM} -f ${PROG} ${OBJS} core

