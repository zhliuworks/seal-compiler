
ASSN = 5
CLASS= compiler principle
LIB= -L/usr/pubsw/lib 

SRC= cgen.cc cgen.h cgen_supp.cc seal-decl.h seal-stmt.h seal-expr.h seal-tree.handcode.h emit.h example.cl README
CSRC= cgen-phase.cc utilities.cc stringtab.cc dumptype.cc tree.cc seal-decl.cc seal-stmt.cc seal-expr.cc seal-lex.cc seal-parse.cc handle_flags.cc 
CFIL= cgen.cc cgen_supp.cc ${CSRC}
OBJS= ${CFIL:.cc=.o}
SEMANT= semant.o
CPPINCLUDE= -I. 


CC=g++
CFLAGS=-g -Wall -Wno-unused -Wno-write-strings -Wno-deprecated ${CPPINCLUDE} -DDEBUG -std=c++11

DEPEND = ${CC} -MM ${CPPINCLUDE}

cgen:	${OBJS} ${SEMANT}
	${CC} ${CFLAGS} ${OBJS} ${SEMANT} ${LIB} -o cgen

.cc.o:
	${CC} ${CFLAGS} -c $<

clean :
	-rm -f *.s ${OBJS} cgen *~ *.a



