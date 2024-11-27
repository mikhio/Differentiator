#include "iog_bin_tree.h"
#include "iog_bin_tree_dumper.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdlib.h>
#include <stdio.h>

IogBTNode_t *iog_BTNodeInit (IogBTData_t data) {
  IogBTNode_t *node = (IogBTNode_t *) calloc(1, sizeof (IogBTNode_t));

  if (node == NULL) {
    fprintf(stderr, RED("BTNodeInitError: can't calloc node\n"));
  } else {
    node->data  = data;
    node->right = NULL;
    node->left  = NULL;
  }

  return node;
}

int iog_BTNodeInsert  (IogBTNode_t *root, IogBTNode_t *node) {
  IOG_ASSERT(root);
  IOG_ASSERT(node);

  if (root->data <= node->data) {
    if (root->right == NULL) {
      root->right = node;
    } else {
      if (iog_BTNodeInsert(root->right, node) != OK) {
        fprintf(stderr, RED("BTNodeInsertError: can't insert to right subtree\n"));
        return ERR;
      }
    }
  } else {
    if (root->left == NULL) {
      root->left = node;
    } else {
      if (iog_BTNodeInsert(root->left, node) != OK) {
        fprintf(stderr, RED("BTNodeInsertError: can't insert to left subtree\n"));
        return ERR;
      }
    }
  }

  return OK;
}

int iog_BTNodeInsertD (IogBTNode_t *root, IogBTData_t data) {
  IOG_ASSERT(root);

  return iog_BTNodeInsert(root, iog_BTNodeInit(data));
}

int iog_BTDestroy (IogBTNode_t **root) {
  IOG_ASSERT(root); 

  if (*root != NULL) {
    iog_BTDestroy(&((*root)->right));
    iog_BTDestroy(&((*root)->left));

    (*root)->data = 0;
  }

  free(*root);
  *root = NULL;

  return OK;
}

int iog_BTDump (const IogBTNode_t *root, IogDebugInfo_t debug, size_t *dumps_count) {
  IOG_ASSERT(dumps_count);

  iog_BTGraphDump(root, debug, dumps_count);

  return OK;
}
