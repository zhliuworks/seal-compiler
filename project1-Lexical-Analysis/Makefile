ASSN = 1
CLASS= compiler-principle

SRC= seal.flex
CSRC= lextest.cc utilities.cc stringtab.cc handle_flags.cc
CGEN= seal-lex.cc
LIBS= parser semant cgen
CFIL= ${CSRC} ${CGEN}
OBJS= ${CFIL:.cc=.o}
OUTPUT= test.output

CPPINCLUDE= -I.


FFLAGS= -d -o seal-lex.cc

CC=g++
CFLAGS= -g -Wall -Wno-unused -Wno-write-strings ${CPPINCLUDE}
FLEX=flex ${FFLAGS}

${OUTPUT}:	lexer
	@rm -f test.output
	-./lexer test.seal >test.output 2>&1 

lexer: ${OBJS} ${SRC}
	${CC} ${CFLAGS} ${OBJS} -o lexer

.cc.o:
	${CC} ${CFLAGS} -c $<

seal-lex.cc: seal.flex 
	${FLEX} seal.flex

clean :
	-rm -f ${OUTPUT} *.s core ${OBJS} lexer seal-lex.cc *~ parser cgen semant


