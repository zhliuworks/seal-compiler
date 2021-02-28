
ASSN = 2
CLASS= compiler-principle

SRC= seal.y seal-tree.handcode.h README
CSRC= parser-phase.cc utilities.cc stringtab.cc dumptype.cc \
      tree.cc seal-decl.cc seal-stmt.cc seal-expr.cc seal-lex.cc  handle_flags.cc 
CGEN= seal-parse.cc
HGEN= seal-parse.h
CFIL= ${CSRC} ${CGEN}
OBJS= ${CFIL:.cc=.o}

CPPINCLUDE= -I.

BFLAGS = -d -v -y -b seal --debug -p seal_yy

CC=g++
CFLAGS=-g -Wall -Wno-unused -Wno-deprecated  -Wno-write-strings -DDEBUG ${CPPINCLUDE}
BISON= bison ${BFLAGS}

parser: ${OBJS} ${HGEN} ${CGEN} 
	${CC} ${CFLAGS} ${OBJS} -o parser

.cc.o:
	${CC} ${CFLAGS} -c $<

seal-parse.cc seal-parse.h: seal.y
	bison ${BFLAGS} seal.y
	mv -f seal.tab.c seal-parse.cc

clean :
	-rm -f  *.s core ${OBJS} ${CGEN}  lexer parser cgen semant *~ *.a *.o 