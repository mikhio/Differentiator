#ifndef IOG_EXPR_PARSER_H
#define IOG_EXPR_PARSER_H

// sigma+ = {"102.1 + -2.2 ", " (9+ 2) - 32 *12"}
// sigma- = {"10 2 + 2", "- 2", "", " "}

// ---- Grammar -----
// Eval       := Polynamial'\0'
// Polynomial := Monomial{['+','-']Monomial}*
// Monomial   := Power{['*','/']Power}*
// Power      := Expr[^Expr]
// Expr       := Space('('Polynomial')' | Double)Space
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
  int p;
};


double GetEval       (IogContext_t *cont);
double GetPolynomial (IogContext_t *cont);
double GetMonomial   (IogContext_t *cont);
double GetPower      (IogContext_t *cont);
double GetExpr       (IogContext_t *cont);
int    GetSpace      (IogContext_t *cont);
double GetDouble     (IogContext_t *cont);
double GetPosDouble  (IogContext_t *cont);
int    GetNumber     (IogContext_t *cont);


#endif // IOG_EXPR_PARSER_H
