#include <stdio.h>
#include <unistd.h>    // for getopt
#include "seal-decl.h"
#include "seal-expr.h"
#include "seal-stmt.h"

extern Program ast_root;      // root of the abstract syntax tree
FILE *fin;                    // input file
extern int optind;  // used for option processing (man 3 getopt for more info)
extern int seal_yyparse(void); // entry point to the AST parser
extern int omerrs;            // syntax check errors
char *curr_filename = "<stdin>";

void handle_flags(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  fin = fopen(argv[optind], "r");
	    if (fin == NULL) {
		cerr << "Could not open input file " << argv[optind] << endl;
		exit(1);
	}
  curr_lineno = 1;
  handle_flags(argc,argv);
  seal_yyparse();
  if(omerrs != 0 || ast_root == NULL){
    cout << "syntax analyze failed. Please make sure syntax parser passed." << endl;
    exit(-1);
  }
  ast_root->semant();
  ast_root->dump_with_types(cout,0);
  fclose(fin);
}

