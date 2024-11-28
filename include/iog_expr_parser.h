#ifndef IOG_EXPR_PARSER_H
#define IOG_EXPR_PARSER_H

// sigma+ = {"102 + 2 ", " (9+ 2) - 32 *12"}
// sigma- = {"10 2 + 2", "- 2", "", " "}

// ---- Grammar -----
// G := E'\0'
// E := T{['+','-']T}*
// T := P{['*','/']P}*
// P := S('('E')' | N)S
// S := [' ', '\n', '\t']*
// N := ['0'-'9']+
// ------------------ 

#define GET_CHAR(context) (context->expr[context->p])

struct IogContext_t {
  char expr[100];
  int p;
};


int GetG (IogContext_t *cont);
int GetE (IogContext_t *cont);
int GetT (IogContext_t *cont);
int GetP (IogContext_t *cont);
int GetS (IogContext_t *cont);
int GetN (IogContext_t *cont);


#endif // IOG_EXPR_PARSER_H
