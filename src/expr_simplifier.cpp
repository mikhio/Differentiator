#include "expr_simplifier.h"

#include "iog_assert.h"
#include "iog_return_codes.h"
#include "cli_colors.h"

#include <stdio.h>
#include <math.h>

int simplify_tree (IogBTNode_t *root) {
  if (root == NULL)
    return OK;

  if (root->type != OPERATION)
    return OK;

  simplify_tree(root->left);
  simplify_tree(root->right);


  switch ((int) root->data) {
    case OP_ADD:
      if (IS_NUMBER(root->left) && IS_NUMBER(root->right)) {
        root->type = NUMBER;
        root->data = root->left->data + root->right->data;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ZERO(root->left)) {
        IogBTNode_t *right_node = root->right;
        iog_BTNodeCopy(root, root->right);
        iog_BTNodeEnNull(right_node);
        iog_BTDestroy(&right_node);
      } else if (IS_ZERO(root->right)) {
        IogBTNode_t *left_node = root->left;
        iog_BTNodeCopy(root, root->left);
        iog_BTNodeEnNull(left_node);
        iog_BTDestroy(&left_node);
      }
      break;
    case OP_SUB:
      if (IS_NUMBER(root->left) && IS_NUMBER(root->right)) {
        root->type = NUMBER;
        root->data = root->left->data - root->right->data;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ZERO(root->right)) {
        IogBTNode_t *left_node = root->left;
        iog_BTNodeCopy(root, root->left);
        iog_BTNodeEnNull(left_node);
        iog_BTDestroy(&left_node);
      }
      break;
    case OP_MUL:
      if (IS_NUMBER(root->left) && IS_NUMBER(root->right)) {
        root->type = NUMBER;
        root->data = root->left->data * root->right->data;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ZERO(root->left) || IS_ZERO(root->right)) {
        root->type = NUMBER;
        root->data = 0;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ONE(root->left)) {
        IogBTNode_t *right_node = root->right;
        iog_BTNodeCopy(root, root->right);
        iog_BTNodeEnNull(right_node);
        iog_BTDestroy(&right_node);
      } else if (IS_ONE(root->right)) {
        IogBTNode_t *left_node = root->left;
        iog_BTNodeCopy(root, root->left);
        iog_BTNodeEnNull(left_node);
        iog_BTDestroy(&left_node);
      }
      break;
    case OP_DIV:
      if (IS_NUMBER(root->left) && IS_NUMBER(root->right)) {
        root->type = NUMBER;
        root->data = root->left->data / root->right->data;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ZERO(root->left)) {
        root->type = NUMBER;
        root->data = 0;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ZERO(root->right)) {
        root->type = NUMBER;
        root->data = INFINITY;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ONE(root->right)) {
        IogBTNode_t *left_node = root->left;
        iog_BTNodeCopy(root, root->left);
        iog_BTNodeEnNull(left_node);
        iog_BTDestroy(&left_node);
      }
      break;
    case OP_POW:
      if (IS_NUMBER(root->left) && IS_NUMBER(root->right)) {
        root->type = NUMBER;
        root->data = pow(root->left->data, root->right->data);

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ZERO(root->left)) {
        root->type = NUMBER;
        root->data = 0;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ZERO(root->right)) {
        root->type = NUMBER;
        root->data = 1;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ONE(root->left)) {
        root->type = NUMBER;
        root->data = 1;

        iog_BTDestroy(&root->left);
        iog_BTDestroy(&root->right);
      } else if (IS_ONE(root->right)) {
        IogBTNode_t *left_node = root->left;
        iog_BTNodeCopy(root, root->left);
        iog_BTNodeEnNull(left_node);
        iog_BTDestroy(&left_node);
      }
      break;
    case OP_NONE:
      fprintf(stderr, MAGENTA("[WARNING]") " Ignoring OP_NONE node\n");
      break;
    default:
      fprintf(stderr, RED("SimplifyError: Undefined opearation %lg in node %p\n"), root->data, root);
      return ERR;

  }

  return OK;
}
