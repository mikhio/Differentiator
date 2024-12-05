#include "iog_bin_tree.h"
#include "iog_bin_tree_dumper.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

IogBTNode_t *iog_BTNodeInit (IogBTData_t data, IogBTNodeType type) {
  IogBTNode_t *node = (IogBTNode_t *) calloc(1, sizeof (IogBTNode_t));

  if (node == NULL) {
    fprintf(stderr, RED("BTNodeInitError: can't calloc node\n"));
  } else {
    node->data  = data;
    node->right = NULL;
    node->left  = NULL;

    node->state = USUAL;
    node->type  = type;
  }

  return node;
}

IogBTNode_t *iog_BTNodeCopy (const IogBTNode_t *node) {
  IOG_ASSERT(node);

  IogBTNode_t *new_node = iog_BTNodeInit(0);
  memcpy(new_node, node, sizeof(IogBTNode_t));

  return new_node;
}

IogBTNode_t *iog_BTCopy (const IogBTNode_t *root) {
  if (root == NULL)
    return NULL;

  IogBTNode_t *new_root = iog_BTNodeCopy(root);

  new_root->right = iog_BTCopy(root->right);
  new_root->left  = iog_BTCopy(root->left);

  return new_root;
}

int iog_BTNodeEnNull (IogBTNode_t *node) {
  if (node != NULL) {
    memset(node, 0, sizeof (IogBTNode_t));
  }

  return OK;
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

int iog_BTNodeInitRight (IogBTNode_t *node, IogBTData_t data, IogBTNodeType) {
  IOG_ASSERT(node);

  if (node->right != NULL) {
    fprintf(stderr, RED("BTNodeInitRightError: Right pointer isn't NULL\n"));
    return ERR;
  }
  node->right = iog_BTNodeInit(data);

  return OK;
}

int iog_BTNodeInitLeft (IogBTNode_t *node, IogBTData_t data, IogBTNodeType type) {
  IOG_ASSERT(node);

  if (node->left != NULL) {
    fprintf(stderr, RED("BTNodeInitLeftError: Left pointer isn't NULL\n"));
    return ERR;
  }

  node->left = iog_BTNodeInit(data, type);

  return OK;
}

int iog_BTNodeHangRight (IogBTNode_t *node, IogBTNode_t *hanging_node) {
  IOG_ASSERT(node);

  if (node->right != NULL) {
    fprintf(stderr, RED("BTNodeHangRightError: Right pointer isn't NULL\n"));
    return ERR;
  }
  node->right = hanging_node;

  return OK;
}

int iog_BTNodeHangLeft (IogBTNode_t *node, IogBTNode_t *hanging_node) {
  IOG_ASSERT(node);

  if (node->left != NULL) {
    fprintf(stderr, RED("BTNodeHangLeftError: Left pointer isn't NULL\n"));
    return ERR;
  }
  node->left = hanging_node;

  return OK;
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
