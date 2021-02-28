#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "seal-io.h"  //includes iostream
#include "seal-decl.h"
#include "seal-stmt.h"
#include "seal-expr.h"
#include "cgen_gc.h"

extern int optind;            // for option processing
extern char *out_filename;    // name of output assembly
extern Program ast_root;             // root of the abstract syntax tree
extern int omerrs;            // syntax errors
extern int semant_errors;     // semant errors
FILE *fin;       // we read the AST from standard input
extern int seal_yyparse(void); // entry point to the AST parser

int cool_yydebug;     // not used, but needed to link with handle_flags
char *curr_filename = "<stdin>";

void handle_flags(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  int firstfile_index;
  fin = fopen(argv[optind], "r");
	    if (fin == NULL) {
		cerr << "Could not open input file " << argv[optind] << endl;
		exit(1);
	}
  curr_lineno = 1;
  handle_flags(argc,argv);
  firstfile_index = optind;

  if (!out_filename && optind < argc) {   // no -o option
      char *dot = strrchr(argv[optind], '.');
      if (dot) *dot = '\0'; // strip off file extension
      out_filename = new char[strlen(argv[optind])+8];
      strcpy(out_filename, argv[optind]);
      strcat(out_filename, ".s");
  }
  // 
  // Don't touch the output file until we know that earlier phases of the
  // compiler have succeeded.
  //
  seal_yyparse();
  if(omerrs != 0 || ast_root == NULL){
    cerr << "syntax analyze failed. Please make sure syntax parser passed." << endl;
    exit(-1);
  }
  ast_root->semant();
  if(semant_errors != 0) {
    cerr << "semant analyze failed. Please make sure semant parser passed." << endl;
    exit(-1);
  }
  if (out_filename) {
      ofstream s(out_filename);
      if (!s) {
        cerr << "Cannot open output file " << out_filename << endl;
        exit(1);
      }
      ast_root->cgen(s);
  } else {
      ast_root->cgen(cout);
  }
  fclose(fin);
}

