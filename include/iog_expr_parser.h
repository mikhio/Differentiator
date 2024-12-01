#ifndef IOG_EXPR_PARSER_H
#define IOG_EXPR_PARSER_H

#include "iog_bin_tree.h"

// sigma+ = {"102.1 + -2.2 ", " (9+ 2) - 32 *12"}
// sigma- = {"10 2 + 2", "- 2", "", " "}

// ---- Grammar -----
// Eval       := Polynamial'\0'
// Polynomial := Monomial{['+','-']Monomial}*
// Monomial   := Power{['*','/']Power}*
// Power      := Expr[^Expr]
// Expr       := Space('('Polynomial')' | Unit)Space
// Unit       := Variable | Double
// Variable   := 'x'
// Space      := [' ', '\n', '\t']*
// Dobule     := ['-']PosDouble
// PosDouble  := Number['.'Number]
// Number     := ['0'-'9']+
// ------------------ 

const double EPSILON = 0.0001;
#define GET_CHAR(context) (context->expr[context->p])

#define IS_EQUAL(x, y) ( (x - y) >= EPSILON ? 1 : 0)

struct IogContext_t {
  char expr[100];
  int  p;
};


int GetEval       (IogContext_t *cont, IogBTNode_t *tree);
int GetPolynomial (IogContext_t *cont, IogBTNode_t *tree);
int GetMonomial   (IogContext_t *cont, IogBTNode_t *tree);
int GetPower      (IogContext_t *cont, IogBTNode_t *tree);
int GetExpr       (IogContext_t *cont, IogBTNode_t *tree);

int GetVariable   (IogContext_t *cont, char *val);

int GetSpace      (IogContext_t *cont);
int GetDouble     (IogContext_t *cont, double *val);
int GetPosDouble  (IogContext_t *cont, double *val);
int GetNumber     (IogContext_t *cont, double *val);


#endif // IOG_EXPR_PARSER_H
