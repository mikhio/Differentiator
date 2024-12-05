#include "differentiator.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>


int differentiate_tree (IogBTNode_t *root) {
  if (root == NULL)
    return ERR_NULLPTR;

  switch ((int) root->type) {
    case NUMBER:
    {
      iog_BTNodeEnNull(root);
      break;
    }
    case VARIABLE:
    {
      root->type = NUMBER;
      root->data = 1;
      break;
    }
    case OPERATION:
    {
      switch ((int) root->data) {
        case  OP_NONE:
          fprintf(stderr, MAGENTA("[WARNING] ") " Ignoring OP_NONE node\n");
          break;
        case OP_ADD:
          differentiate_tree(root->right);
          differentiate_tree(root->left);
          break;
        case OP_SUB:
          differentiate_tree(root->right);
          differentiate_tree(root->left);
          break;
        case OP_MUL:
        {
          root->data = OP_ADD;

          IogBTNode_t *left_node = iog_BTNodeInit(OP_MUL, OPERATION);
          IogBTNode_t *right_node = iog_BTNodeInit(OP_MUL, OPERATION);
          
          left_node->left = iog_BTClone(root->left);
          differentiate_tree(left_node->left);
          left_node->right = root->right;

          right_node->left = iog_BTClone(root->right);
          differentiate_tree(right_node->left);
          right_node->right = root->left;

          root->left  = left_node;
          root->right = right_node;
          break;
        }
        case OP_DIV:
        {
          spreadDivision(root);
          convDivToPow(root->right);
          differentiate_tree(root);
          break;
        }
        case OP_POW:
        {
          IOG_ASSERT(root->right->type == NUMBER);
          
          root->data = OP_MUL;

          IogBTData_t deg = root->right->data;
          iog_BTDestroy(&root->right);
          root->right = iog_BTNodeInit(OP_MUL, OPERATION);

          root->right->left = iog_BTNodeInit(OP_POW, OPERATION);
          root->right->left->left = root->left;
          root->right->left->right = iog_BTNodeInit(deg-1, NUMBER);

          root->right->right = iog_BTClone(root->left);
          differentiate_tree(root->right->right);

          root->left = iog_BTNodeInit(deg, NUMBER);
          break;
        }
        default:
          fprintf(stderr, RED("DifferentiateError: Undefined operation type %lg of node %p\n"), root->data, root);
         return ERR; 
      }
      break;
    }
    default:
    {
      fprintf(stderr, RED("DifferentiateError: Undefined type %d of node %p\n"), root->type, root);
      return ERR;
    }
  }

  return OK;
}

static int spreadDivision (IogBTNode_t *root) {
  IOG_ASSERT(root);
  IOG_ASSERT(root->type == OPERATION);
  IOG_ASSERT(root->data == OP_DIV);
  
  IogBTNode_t *new_node = iog_BTNodeInit(OP_DIV, OPERATION);
  new_node->left = iog_BTNodeInit(1);
  new_node->right = root->right;

  root->right = new_node;
  root->data = OP_MUL;

  return OK;
}

static int convDivToPow (IogBTNode_t *root) {
  IOG_ASSERT(root);
  IOG_ASSERT(root->type == OPERATION);
  IOG_ASSERT(root->data == OP_DIV);

  IOG_ASSERT(root->left->type == NUMBER);
  IOG_ASSERT(root->left->data == 1);

  root->data = OP_POW;

  IogBTNode_t *left_node = root->left; 
  root->left = root->right;
  root->right = left_node;

  iog_BTNodeEnNull(root->right);
  root->right->type = NUMBER;
  root->right->data = -1;

  return OK;
}

