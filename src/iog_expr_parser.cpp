#include "iog_expr_parser.h"

#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>
#include <math.h>

double GetEval (IogContext_t *cont) {
  double val = GetPolynomial(cont);

  if (GET_CHAR(cont) != '\0') {
    IOG_ASSERT(!"SyntaxError: No \0 symbols in the end");
  }
  (cont->p)++;

  return val;
}

double GetPolynomial (IogContext_t *cont) {
  double val = GetMonomial(cont);


  while ((GET_CHAR(cont) == '+') ||(GET_CHAR(cont) == '-')) {
    char op = GET_CHAR(cont);

    (cont->p)++;

    double val2 = GetMonomial(cont);

    if (op == '+')
      val += val2;
    else
      val -= val2;
  }


  return val;
}

double GetMonomial (IogContext_t *cont) {
  double val = GetPower(cont);

  while ((GET_CHAR(cont) == '*') ||(GET_CHAR(cont) == '/')) {
    char op = GET_CHAR(cont);

    (cont->p)++;

    double val2 = GetPower(cont);

    if (op == '*') {
      val *= val2;
    } else {
      val /= val2;
    }
  }

  return val;
}

double GetPower (IogContext_t *cont) {
  double val = GetExpr(cont);

  if (GET_CHAR(cont) == '^') {
    (cont->p)++;

    double val2 = GetExpr(cont);

    val = pow(val, val2);
  }

  return val;
}

double GetExpr (IogContext_t *cont) {
  GetSpace(cont);

  double val = 0;
  if (GET_CHAR(cont) == '(') {
    (cont->p)++;

    val = GetPolynomial(cont);

    if (GET_CHAR(cont) != ')')
      IOG_ASSERT(!"SyntaxError: No closing braket");

    (cont->p)++;
  } else {
    val = GetDouble(cont);
  }

  GetSpace(cont);


  return val;
}

int GetSpace (IogContext_t *cont) {
  while ((GET_CHAR(cont) == ' ') || (GET_CHAR(cont) == '\n') || (GET_CHAR(cont) == '\t')) {
    (cont->p)++;
  }

  return 0;
}

double GetDouble (IogContext_t *cont) {
  double val = 0;

  if (GET_CHAR(cont) == '-') {
    (cont->p)++;
    val -= GetPosDouble(cont);
  } else {
    val = GetPosDouble(cont);
  }


  return val;
}

double GetPosDouble (IogContext_t *cont) {
  double val = (double) GetNumber(cont);

  if (GET_CHAR(cont) == '.') {
    (cont->p)++;

    double val2 = (double) GetNumber(cont);

    while (val2 > 1) {
      val2 *= 0.1;
    } 
    
    val += val2;
  }
  
  return val;
}

int GetNumber (IogContext_t *cont) {
  int val = 0;
  int old_p = cont->p;

  while (('0' <= GET_CHAR(cont)) && (GET_CHAR(cont) <= '9')) {
    val = val * 10 + GET_CHAR(cont) - '0';
    (cont->p)++;
  }

  if (old_p == cont->p) {
    /*fprintf(stderr, "expr: '%s', p: %d, char: %c\n", 
        cont->expr, cont->p, GET_CHAR(cont)
    );*/
    IOG_ASSERT(!"SynataxError: No num symbols");
  }

  return val;
}
