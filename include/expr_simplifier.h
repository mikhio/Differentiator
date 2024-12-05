#ifndef EXPR_SIMPLIFIER_H
#define EXPR_SIMPLIFIER_H

#include "iog_bin_tree.h"

#define IS_NUMBER(node) (node->type == NUMBER)
#define IS_ZERO(node) ((IS_NUMBER(node)) && (node->data == 0))
#define IS_ONE(node) ((IS_NUMBER(node)) && (node->data == 1))

int simplify_tree (IogBTNode_t *root);

#endif // EXPR_SIMPLIFIER_H
