//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"

//////////////////////////////////////////////////////////////////////////////
//
//  parser-phase.cc
//
//  Reads a SEAL token stream from a file and builds the abstract syntax tree.
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>     // for Linux system
#include <unistd.h>    // for getopt
#include "seal-io.h"  //includes iostream
#include "seal-decl.h"
#include "seal-stmt.h"
#include "seal-expr.h"
#include "utilities.h"  // for fatal_error
#include "seal-parse.h"


//
// These globals keep everything working.
//
FILE *token_file = stdin;		// we read from this file
extern Program ast_root;	 // the AST produced by the parse

extern int optind;  // used for option processing (man 3 getopt for more info)

char *curr_filename = "<stdin>";
FILE *fin;

extern int omerrs;             // a count of lex and parse errors

extern int seal_yyparse();
void handle_flags(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    fin = fopen(argv[optind], "r");
	    if (fin == NULL) {
		cerr << "Could not open input file " << argv[optind] << endl;
		exit(1);
	}
    curr_lineno = 1;
    handle_flags(argc, argv);
    seal_yyparse();
    if (omerrs != 0) {
	    cerr << "Compilation halted due to lex and parse errors\n";
	    exit(1);
    }
    if(ast_root == NULL) {
        cerr << "ast_root must be initialized.\n";
	    exit(1);
    }
    ast_root->dump_with_types(cout,0);
    fclose(fin);
    return 0;
}

