
ASSN = 4
CLASS= compiler principle
LIB= -L/usr/pubsw/lib 
AR= gar
ARCHIVE_NEW= -cr
RANLIB= gar -qs

SRC= semant.cc semant.h seal-decl.h seal-expr.h seal-stmt.h seal-tree.handcode.h 
CSRC= semant-phase.cc handle_flags.cc  seal-lex.cc seal-parse.cc utilities.cc stringtab.cc dumptype.cc tree.cc seal-expr.cc seal-stmt.cc seal-decl.cc  
TSRC= seal-tree.aps
CFIL= semant.cc ${CSRC} ${CGEN} 
LSRC= Makefile
OBJS= ${CFIL:.cc=.o}

CPPINCLUDE= -I. 

CC=g++
CFLAGS=-g -Wall -Wno-unused -Wno-write-strings -Wno-deprecated ${CPPINCLUDE} -DDEBUG

SEMANT_OBJS := ${OBJS}

semant:  ${SEMANT_OBJS}
	${CC} ${CFLAGS} ${SEMANT_OBJS} ${LIB} -o semant

.cc.o:
	${CC} ${CFLAGS} -c $<

clean :
	-rm -f ${OUTPUT} *.s ${OBJS} semant  *~ *.a *.o