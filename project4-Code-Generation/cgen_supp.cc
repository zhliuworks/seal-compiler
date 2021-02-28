#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "stringtab.h"
#include "utilities.h"
static int ascii = 0;

void emit_string_constant(ostream& str, char* s)
{
  str << "\"";

  print_escaped_string(str, s);
  str << "\"" << endl;
}


