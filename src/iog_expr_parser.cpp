#include "iog_expr_parser.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>
#include <math.h>

int GetEval (IogContext_t *cont, IogBTNode_t *tree) {
  IOG_ASSERT(cont);
  IOG_ASSERT(tree);

  GetPolynomial(cont, tree);

  if (GET_CHAR(cont) != '\0') {
    IOG_ASSERT(!"SyntaxError: No \0 symbols in the end");
  }
  (cont->p)++;

  return OK;
}

int GetPolynomial (IogContext_t *cont, IogBTNode_t *tree) {
  IOG_ASSERT(cont);
  IOG_ASSERT(tree);

  GetMonomial(cont, tree);

  while ((GET_CHAR(cont) == '+') ||(GET_CHAR(cont) == '-')) {
    char op = GET_CHAR(cont);

    IogBTNode_t *old_tree = iog_BTNodeCopy(tree); 
    iog_BTNodeEnNull(tree);

    if (op == '+') {
      tree->type = OPERATION;
      tree->data = OP_ADD;
    }
    else {
      tree->type = OPERATION;
      tree->data = OP_SUB;
    }

    tree->left = old_tree;

    (cont->p)++;

    tree->right = iog_BTNodeInit(0);
    GetMonomial(cont, tree->right);

  }


  return OK;
}

int GetMonomial (IogContext_t *cont, IogBTNode_t *tree) {
  IOG_ASSERT(cont);
  IOG_ASSERT(tree);

  GetPower(cont, tree);

  while ((GET_CHAR(cont) == '*') ||(GET_CHAR(cont) == '/')) {
    char op = GET_CHAR(cont);

    IogBTNode_t *old_tree = iog_BTNodeCopy(tree);
    iog_BTNodeEnNull(tree);

    if (op == '*') {
      tree->type = OPERATION;
      tree->data= OP_MUL;
    } else {
      tree->type = OPERATION;
      tree->data= OP_DIV;
    }

    tree->left = old_tree;

    (cont->p)++;

    tree->right = iog_BTNodeInit(0);
    GetPower(cont, tree->right);

  }

  return OK;
}

int GetPower (IogContext_t *cont, IogBTNode_t *tree) {
  IOG_ASSERT(cont);
  IOG_ASSERT(tree);

  GetExpr(cont, tree);

  if (GET_CHAR(cont) == '^') {
    (cont->p)++;

    IogBTNode_t *old_tree = iog_BTNodeCopy(tree);
    iog_BTNodeEnNull(tree);

    tree->type = OPERATION;
    tree->data = OP_DIV;

    tree->left = old_tree;

    tree->right = iog_BTNodeInit(0);
    GetExpr(cont, tree->right);

  }

  return OK;
}

int GetExpr (IogContext_t *cont, IogBTNode_t *tree) {
  IOG_ASSERT(cont);
  IOG_ASSERT(tree);

  GetSpace(cont);

  if (GET_CHAR(cont) == '(') {
    (cont->p)++;

    GetPolynomial(cont, tree);

    if (GET_CHAR(cont) != ')')
      IOG_ASSERT(!"SyntaxError: No closing braket");

    (cont->p)++;
  } else {
    tree->type = NUMBER;
    GetDouble(cont, &tree->data);
  }

  GetSpace(cont);


  return OK;
}

int GetSpace (IogContext_t *cont) {
  IOG_ASSERT(cont);

  while ((GET_CHAR(cont) == ' ') || (GET_CHAR(cont) == '\n') || (GET_CHAR(cont) == '\t')) {
    (cont->p)++;
  }

  return OK;
}

int GetDouble (IogContext_t *cont, double *val) {
  IOG_ASSERT(cont);
  IOG_ASSERT(val);

  if (GET_CHAR(cont) == '-') {
    (cont->p)++;
    GetPosDouble(cont, val);
    *val = -(*val);
  } else {
    GetPosDouble(cont, val);
  }


  return OK;
}

int GetPosDouble (IogContext_t *cont, double *val) {
  IOG_ASSERT(cont);
  IOG_ASSERT(val);

  GetNumber(cont, val);

  if (GET_CHAR(cont) == '.') {
    (cont->p)++;

    double val2 = 0;
    GetNumber(cont, &val2);

    while (val2 > 1) {
      val2 *= 0.1;
    } 
    
    *val += val2;
  }
  
  return OK;
}

int GetNumber (IogContext_t *cont, double *val) {
  IOG_ASSERT(cont);
  IOG_ASSERT(val);

  int old_p = cont->p;

  while (('0' <= GET_CHAR(cont)) && (GET_CHAR(cont) <= '9')) {
    *val = *val * 10 + GET_CHAR(cont) - '0';
    (cont->p)++;
  }

  if (old_p == cont->p) {
    /*fprintf(stderr, "expr: '%s', p: %d, char: %c\n", 
        cont->expr, cont->p, GET_CHAR(cont)
    );*/
    IOG_ASSERT(!"SynataxError: No num symbols");
  }

  return OK;
}
