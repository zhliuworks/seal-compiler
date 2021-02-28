//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"

//////////////////////////////////////////////////////////////////////////////
//
//  utilities.c
//
//  General support code for lexer and parser.
//
//  This file contains:
//      fatal_error            print an error message and exit
//      print_escaped_string   print a string showing escape characters
//      print_seal_token       print a seal token and its semantic value
//      dump_seal_token        dump a readable token representation
//      strdup                 duplicate a string (missing from some libraries)
//
///////////////////////////////////////////////////////////////////////////////

#include "seal-io.h"     // for cerr, <<, manipulators
#include <ctype.h>       // for isprint
#include "seal-parse.h"  // defines tokens
#include "stringtab.h"   // Symbol <-> String conversions
#include "utilities.h"

// #define CHECK_TABLES

// sm: fixed an off-by-one error here; code assumed there were 80 spaces, but
// in fact only 79 spaces were there; I've made it 80 now
//                                1         2         3         4         5         6         7
//                      01234567890123456789012345678901234567890123456789012345678901234567890123456789
static char *padding = "                                                                                ";      // 80 spaces for padding

void fatal_error(char *msg)
{
   cerr << msg;
   exit(1);
}


void print_escaped_string(ostream& str, const char *s)
{
  while (*s) {
    switch (*s) {
    case '\\' : str << "\\\\"; break;
    case '\"' : str << "\\\""; break;
    case '\n' : str << "\\n"; break;
    case '\t' : str << "\\t"; break;
    case '\b' : str << "\\b"; break;
    case '\f' : str << "\\f"; break;

    default:
      if (isprint(*s))
	str << *s;
      else 
	// 
	// Unprintable characters are printed using octal equivalents.
	// To get the sign of the octal number correct, the character
	// must be cast to an unsigned char before coverting it to an
	// integer.
	//
	str << '\\' << oct << setfill('0') << setw(3)
	    << (int) ((unsigned char) (*s))
	    << dec << setfill(' ');
      break;
    }
    s++;
  }
}

//
// The following two functions are used for debugging the parser.
//
char *seal_token_to_string(int tok)
{
  switch (tok) {
  case 0:             return("EOF");          break;
  case (IF):          return("if");           break;
  case (ELSE):        return("else");         break;
  case (WHILE):       return("while");        break;
  case (FOR):         return("for");          break;
  case (BREAK):       return("break");        break;
  case (CONTINUE):    return("continue");     break;
  case (FUNC):        return("func");         break;
  case (RETURN):      return("return");       break;
  case (CONST_BOOL):  return("CONST_BOOL");   break;
  case (CONST_INT):   return("CONST_INT");    break;
  case (CONST_STRING):return("CONST_STRING"); break;
  case (CONST_FLOAT): return("CONST_FLOAT");  break;
  case (OBJECTID):    return("OBJECTID");     break;
  case (TYPEID):      return("TYPEID");     break;
  case (VAR):         return("var");          break;
  case (STRUCT):      return("struct");       break;
  case (ERROR):       return("ERROR");        break;
  case (AND):         return("&&");           break;
  case (OR):          return("||");           break;
  case (EQUAL):       return("==");           break;
  case (NE):          return("!=");           break;
  case (GE):          return(">=");           break;
  case (LE):          return("<=");           break;

  case '+': return("'+'"); break;
  case '/': return("'/'"); break;
  case '-': return("'-'"); break;
  case '*': return("'*'"); break;
  case '=': return("'='"); break;
  case '<': return("'<'"); break;
  case '.': return("'.'"); break;
  case '~': return("'~'"); break;
  case ',': return("','"); break;
  case ';': return("';'"); break;
  case ':': return("':'"); break;
  case '(': return("'('"); break;
  case ')': return("')'"); break;
  case '@': return("'@'"); break;
  case '{': return("'{'"); break;
  case '}': return("'}'"); break;
  case '%': return("'%'"); break;
  case '>': return("'>'"); break;
  case '&': return("'&'"); break;
  case '!': return("'!'"); break;
  case '^': return("'^'"); break;
  case '|': return("'|'"); break;
  default:  return("<Invalid Token>");
  }
}

void print_seal_token(int tok)
{

  cerr << seal_token_to_string(tok);

  switch (tok) {
  case (CONST_STRING):
    cerr << " = ";
    cerr << " \"";
    print_escaped_string(cerr, seal_yylval.symbol->get_string());
    cerr << "\"";
#ifdef CHECK_TABLES
    stringtable.lookup_string(seal_yylval.symbol->get_string());
#endif
    break;
  case (CONST_INT):
    cerr << " = " << seal_yylval.symbol;
#ifdef CHECK_TABLES
    inttable.lookup_string(seal_yylval.symbol->get_string());
#endif
    break;
  case (CONST_FLOAT):
    cerr << " = " << seal_yylval.symbol;
#ifdef CHECK_TABLES
    floattable.lookup_string(seal_yylval.symbol->get_string());
#endif
    break;
  case (CONST_BOOL):
    cerr << (seal_yylval.boolean ? " = true" : " = false");
    break;
  case (OBJECTID):
    cerr << " = " << seal_yylval.symbol;
#ifdef CHECK_TABLES
    idtable.lookup_string(seal_yylval.symbol->get_string());
#endif
    break;
  case (TYPEID):
    cerr << " = " << seal_yylval.symbol;
#ifdef CHECK_TABLES
    idtable.lookup_string(seal_yylval.symbol->get_string());
#endif
    break;
  case (ERROR): 
    cerr << " = ";
    print_escaped_string(cerr, seal_yylval.error_msg);
    break;
  }
}

// dump the token in format readable by the sceond phase token lexer
void dump_seal_token(ostream& out, int lineno, int token, YYSTYPE yylval)
{
    out << "#" << lineno << " " << seal_token_to_string(token);

    switch (token) {
    case (CONST_STRING):
	out << " \"";
	print_escaped_string(out, seal_yylval.symbol->get_string());
	out << "\"";
#ifdef CHECK_TABLES
	stringtable.lookup_string(seal_yylval.symbol->get_string());
#endif
	break;
    case (CONST_INT):
	out << " " << seal_yylval.symbol;
#ifdef CHECK_TABLES
	inttable.lookup_string(seal_yylval.symbol->get_string());
#endif
	break;
    case (CONST_FLOAT):
	out << " " << seal_yylval.symbol;
#ifdef CHECK_TABLES
	floattable.lookup_string(seal_yylval.symbol->get_string());
#endif
	break;
    case (CONST_BOOL):
	out << (seal_yylval.boolean ? " true" : " false");
	break;
    case (OBJECTID):
	out << " " << seal_yylval.symbol;
#ifdef CHECK_TABLES
	idtable.lookup_string(seal_yylval.symbol->get_string());
#endif
	break;
    case (TYPEID):
	out << " " << seal_yylval.symbol;
#ifdef CHECK_TABLES
	idtable.lookup_string(seal_yylval.symbol->get_string());
#endif
	break;
    case (ERROR): 
        // sm: I've changed assignment 2 so students are supposed to
        // *not* coalesce error characters into one string; therefore,
        // if we see an "empty" string here, we can safely assume the
        // lexer is reporting an occurrance of an illegal NUL in the
        // input stream
        if (seal_yylval.error_msg[0] == 0) {
          out << " \"\\000\"";
        }
        else {
          out << " \"";
          print_escaped_string(out, seal_yylval.error_msg);
          out << "\"";
          break;
        }
    }
    out << endl;
}

//
// Decstations don't have strdup in the library.
//
char *strdup(const char *s)
{
  char *news;

  if (s == NULL) return(NULL);

  news = (char *)malloc(strlen(s)+1);
  strcpy(news, s);
  return(news);
}

///////////////////////////////////////////////////////////////////////////
//
// pad
//
// function to add pad
//
///////////////////////////////////////////////////////////////////////////
char *pad(int n) {
    if (n > 80) return padding;
    if (n <= 0)  return "";
    return padding+(80-n);
}
