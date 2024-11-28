#include "iog_expr_parser.h"

#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>

int GetG (IogContext_t *cont) {
  int val = GetE(cont);

  if (GET_CHAR(cont) != '\0') {
    IOG_ASSERT(!"SyntaxError: No \0 symbols in the end");
  }
  (cont->p)++;

  return val;
}

int GetE (IogContext_t *cont) {
  int val = GetT(cont);


  while ((GET_CHAR(cont) == '+') ||(GET_CHAR(cont) == '-')) {
    char op = GET_CHAR(cont);

    (cont->p)++;

    int val2 = GetT(cont);

    if (op == '+')
      val += val2;
    else
      val -= val2;
  }

  return val;
}

int GetT (IogContext_t *cont) {
  int val = GetP(cont);

  while ((GET_CHAR(cont) == '*') ||(GET_CHAR(cont) == '/')) {
    char op = GET_CHAR(cont);

    (cont->p)++;

    int val2 = GetP(cont);

    if (op == '*')
      val *= val2;
    else
      val /= val2;
  }

  return val;
}

int GetP (IogContext_t *cont) {
  GetS(cont);

  int val = 0;
  if (GET_CHAR(cont) == '(') {
    (cont->p)++;


    val = GetE(cont);

    if (GET_CHAR(cont) != ')')
      IOG_ASSERT(!"SyntaxError: No closing braket");

    (cont->p)++;
  } else {
    val = GetN(cont);
  }

  GetS(cont);

  return val;
}

int GetS (IogContext_t *cont) {
  while ((GET_CHAR(cont) == ' ') || (GET_CHAR(cont) == '\n') || (GET_CHAR(cont) == '\t')) {
    (cont->p)++;
  }

  return 0;
}

int GetN (IogContext_t *cont) {
  int val = 0;
  int old_p = cont->p;

  while (('0' <= GET_CHAR(cont)) && (GET_CHAR(cont) <= '9')) {
    val = val * 10 + GET_CHAR(cont) - '0';
    (cont->p)++;
  }

  if (old_p == cont->p) {
    fprintf(stderr, "expr: '%s', p: %d, char: %c\n", 
        cont->expr, cont->p, GET_CHAR(cont)
    );
    IOG_ASSERT(!"SynataxError: No num symbols");
  }

  return val;
}
