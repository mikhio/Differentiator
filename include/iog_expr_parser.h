#ifndef IOG_EXPR_PARSER_H
#define IOG_EXPR_PARSER_H

// sigma+ = {"102 + 2 ", " (9+ 2) - 32 *12"}
// sigma- = {"10 2 + 2", "- 2", "", " "}

// ---- Grammar -----
// G := E'\0'
// E := T{['+','-']T}*
// T := P{['*','/']P}*
// P := S('('E')' | Z)S
// S := [' ', '\n', '\t']*
// Q := ['-']D
// D := N['.'N]
// N := ['0'-'9']+
// ------------------ 

const double EPSILON = 0.0001;
#define GET_CHAR(context) (context->expr[context->p])

#define IS_EQUAL(x, y) ( (x - y) >= EPSILON ? 1 : 0)

struct IogContext_t {
  char expr[100];
  int p;
};


double GetG (IogContext_t *cont);
double GetE (IogContext_t *cont);
double GetT (IogContext_t *cont);
double GetP (IogContext_t *cont);
double GetQ (IogContext_t *cont);
double GetD (IogContext_t *cont);

int GetS (IogContext_t *cont);
int GetN (IogContext_t *cont);


#endif // IOG_EXPR_PARSER_H
