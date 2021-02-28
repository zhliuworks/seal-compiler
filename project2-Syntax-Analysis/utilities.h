// -*-Mode: C++;-*-
//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "seal-io.h"

extern char *seal_token_to_string(int tok);
extern void print_seal_token(int tok);
extern void fatal_error(char *);
extern void print_escaped_string(ostream& str, const char *s);
extern char *pad(int);
/*  On some machines strdup is not in the standard library. */
//char *strdup(const char *s);

#endif
