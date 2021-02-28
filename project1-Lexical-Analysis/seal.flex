 /*
  *  The scanner definition for seal.
  */

 /*
  *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
  *  output, so headers and global definitions are placed here to be visible
  * to the code in the file.  Don't remove anything that was here initially
  */
%{

#include <seal-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <stdint.h>
#include <stdlib.h>

/* The compiler assumes these identifiers. */
#define yylval seal_yylval
#define yylex  seal_yylex

/* Max size of string constants */
#define MAX_STR_CONST 256
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the seal compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE seal_yylval;

/*
 *  Add Your own definitions here
 */


%}

%option noyywrap

STR1 [^\n]*"\\\n"
STR2 [^\n]*"\n"

%%

\/\*(?:[^\*]|\*+[^\/\*])*\*+\/ {
  for(int i=0; i<yyleng; i++) {
    if(yytext[i] == '\n') {
      curr_lineno += 1;
    }
  }
}

\/\*(?:[^\*]|\*+[^\/\*])* {
  for(int i=0; i<yyleng; i++) {
    if(yytext[i] == '\n') {
      curr_lineno += 1;
    }
  }
  strcpy(seal_yylval.error_msg, "EOF in comment");
  return (ERROR);  
}

"//".* {}

"*/" {
  strcpy(seal_yylval.error_msg, "Unmatched */");
  return (ERROR);  
}

"if" {return (IF);}
"else" {return (ELSE);}
"while" {return (WHILE);}
"for" {return (FOR);}
"break" {return (BREAK);}
"continue" {return (CONTINUE);}
"func" {return (FUNC);}
"return" {return (RETURN);}
"var" {return (VAR);}
"true"|"false" {
  if(yytext[0] == 't') {
    seal_yylval.boolean = true;
  } else {
    seal_yylval.boolean = false;
  }
  return (CONST_BOOL);
}

"Int"|"Float"|"String"|"Bool"|"Void" {
  seal_yylval.symbol = idtable.add_string(yytext);
  return (TYPEID);
}

"&&" {return (AND);}
"||" {return (OR);}
"==" {return (EQUAL);}
"!=" {return (NE);}
">=" {return (GE);}
"<=" {return (LE);}
"+" {return ('+');}
"/" {return ('/');}
"-" {return ('-');}
"*" {return ('*');}
"=" {return ('=');}
"<" {return ('<');}
"~" {return ('~');}
"," {return (',');}
";" {return (';');}
":" {return (':');}
"(" {return ('(');}
")" {return (')');}
"{" {return ('{');}
"}" {return ('}');}
"%" {return ('%');}
">" {return ('>');}
"&" {return ('&');}
"!" {return ('!');}
"^" {return ('^');}
"|" {return ('|');}
" " {}
"\t" {}

[a-zA-Z][a-zA-Z0-9_]* {
  if(yytext[0] >= 'A' && yytext[0] <= 'Z') {
    strcpy(seal_yylval.error_msg, "illegal TYPEID ");
    strcat(seal_yylval.error_msg, yytext);
    return (ERROR);
  }
  seal_yylval.symbol = idtable.add_string(yytext);
  return (OBJECTID);
}

[0-9]+ {
  seal_yylval.symbol = inttable.add_string(yytext);
  return (CONST_INT);
}

0[xX][0-9a-fA-F]+ {
  int num = 0;
  int curr;
  for(int i=2; i<yyleng; i++) {
    if(yytext[i] >= '0' && yytext[i] <= '9') {
      curr = yytext[i] - '0';
    } else if (yytext[i] >= 'a' && yytext[i] <= 'f') {
      curr = yytext[i] - 'a' + 10;
    } else {
      curr = yytext[i] - 'A' + 10;
    }
    num = num * 16 + curr;
  }
  char temp[50];
  char decstr[50];
  int n;
  for(n=0; num>0; n++) {
    temp[n] = '0' + num % 10;
    num /= 10;
  }
  for(int i=0; i<n; i++) {
    decstr[i] = temp[n - 1 - i];
  }
  seal_yylval.symbol = inttable.add_string(decstr);
  return (CONST_INT);  
}

[0-9]"."[0-9]+ {
  seal_yylval.symbol = floattable.add_string(yytext);
  return (CONST_FLOAT);
}

[1-9][0-9]+"."[0-9]+ {
  seal_yylval.symbol = floattable.add_string(yytext);
  return (CONST_FLOAT);
}

"\""[^\n"]*"\n" {
  curr_lineno += 1;
  strcpy(seal_yylval.error_msg, "newline in quotation must use a '\\'");
  return (ERROR);
}

"\""{STR1}*[^\n]*"\"" {
  if(yyleng > 256) {
    strcpy(seal_yylval.error_msg, "The length of string exceeds a maximum of 256");
    return (ERROR);    
  }
  char str[500];
  int n = 0;
  for(int i=1; i<yyleng-1; i++) {
    if(yytext[i] == '\\') {
      if(yytext[i + 1] == '0') {
        strcpy(seal_yylval.error_msg, "String contains null character '\\0'");
        return (ERROR);
      } else if(yytext[i + 1] == '\n') {
        curr_lineno += 1;
        str[n] = '\n';
        n ++;
        i ++;
      } else if(yytext[i + 1] == 't') {
        str[n] = '\t';
        n ++;
        i ++;        
      } else if(yytext[i + 1] == 'b') {
        str[n] = '\b';
        n ++;
        i ++;        
      } else if(yytext[i + 1] == 'f') {
        str[n] = '\f';
        n ++;
        i ++;        
      } else if(yytext[i + 1] == 'n') {
        str[n] = '\n';
        n ++;
        i ++;        
      } else {
        str[n] = yytext[i + 1];
        n ++;
        i ++;
      }    
    } else {
      str[n] = yytext[i];
      n ++;
    }
  }
  char *out = new char[n];
  strncpy(out, str, n);
  seal_yylval.symbol = stringtable.add_string(out);
  return (CONST_STRING);
}

"`"{STR2}*[^\n]*"`" {
  if(yyleng > 256) {
    strcpy(seal_yylval.error_msg, "The length of string exceeds a maximum of 256");
    return (ERROR);    
  }
  char str[500];
  int n = 0;
  for(int i=1; i<yyleng-1; i++) {
    if(yytext[i] == '\n') {
      curr_lineno += 1;      
    } 
    str[n] = yytext[i];
    n ++;
  }
  char *out = new char[n];
  strncpy(out, str, n);
  seal_yylval.symbol = stringtable.add_string(out);
  return (CONST_STRING);
}

"\n" {curr_lineno += 1;}

"\"" {
	strcpy(seal_yylval.error_msg, "EOF in string constant");
	return (ERROR);
}

.	{
	strcpy(seal_yylval.error_msg, yytext);
	return (ERROR);
}

%%
