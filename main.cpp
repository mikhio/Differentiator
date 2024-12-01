#include "iog_expr_parser.h"
#include "iog_bin_tree.h"

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

  iog_BTDestroy(&tree);

  return 0;
}
