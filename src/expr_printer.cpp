#include "expr_printer.h"
#include "iog_bin_tree.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>

int print_expression (FILE *stream, const IogBTNode_t *root) {
  if (root == NULL)
    return OK;
  
  switch ((int) root->type) {
    case NUMBER:
    {
      fprintf(stream, "%lg", root->data);
      break;
    }
    case VARIABLE:
    {
      fprintf(stream, "%c", (char) root->data);
      break;
    }
    case OPERATION:
    {
      fprintf(stream, "(");
      print_expression(stream, root->left);
      fprintf(stream, " %c ", (char) root->data);
      print_expression(stream, root->right);
      fprintf(stream, ")");
      break;
    }
    default:
    {
      fprintf(stderr, RED("PrintExprError: Undefined type %d of node %p\n"), root->type, root);
      return ERR;
    }
  }

  return OK;
}
