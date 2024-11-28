#include "iog_expr_parser.h"

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
  while (1) {
    IogContext_t cont = {"", 0};
    printf("Expr: ");
    IOG_ASSERT(!readLine(cont.expr, sizeof(cont.expr)));
    //fprintf(stderr, "ret %s\n", cont.expr);

    printf(BLACK("Result: %lg\n"), GetG(&cont));
  }

  return 0;
}
