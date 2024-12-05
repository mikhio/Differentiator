#include "iog_expr_parser.h"
#include "iog_bin_tree.h"
#include "differentiator.h"
#include "expr_printer.h"
#include "expr_simplifier.h"

#include "cli_colors.h"
#include "iog_assert.h"

#include <stdio.h>

int readLine (char *str, size_t max_len) {
  char *ptr = str;

  *ptr = getchar();
  while ((*ptr != '\n') && (*ptr != EOF) && ((ptr - str) < max_len)) {
    ptr++;
    *ptr = getchar();
  }

  if ((ptr - str) == max_len)
    return 1;

  *ptr = '\0'; 

  return 0;
}

int main(const int argc, const char *argv[]) {
  IogContext_t cont = {"", 0};
  IogBTNode_t *tree = iog_BTNodeInit(0);

  printf("Expr: ");
  IOG_ASSERT(!readLine(cont.expr, sizeof(cont.expr)));
  //fprintf(stderr, "ret %s\n", cont.expr);

  GetEval(&cont, tree);

  size_t dumps_count = 0;
  IOG_BT_DUMP(tree, &dumps_count);

  IogBTNode_t *diff_tree = iog_BTClone(tree);
  differentiate_tree(diff_tree);
  IOG_BT_DUMP(diff_tree, &dumps_count);

  IogBTNode_t *simp_tree = iog_BTClone(diff_tree);
  simplify_tree(simp_tree);
  IOG_BT_DUMP(simp_tree, &dumps_count);

  fprintf(stdout, "IN EXPR:   ");
  print_expression(stdout, tree);
  fprintf(stdout, "\n");

  fprintf(stdout, "DIFF EXPR: ");
  print_expression(stdout, diff_tree);
  fprintf(stdout, "\n");

  fprintf(stdout, "SIMP EXPR: ");
  print_expression(stdout, simp_tree);
  fprintf(stdout, "\n");

  iog_BTDestroy(&tree);
  iog_BTDestroy(&diff_tree);
  iog_BTDestroy(&simp_tree);

  return 0;
}
