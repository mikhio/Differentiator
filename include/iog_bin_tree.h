#ifndef IOG_BIN_TREE_H
#define IOG_BIN_TREE_H

#include "iog_return_codes.h"

#include <stdlib.h>

typedef double IogBTData_t;

#define IOG_BT_DUMP(root, dumps_count) {\
  iog_BTDump(root, {#root, __FILE__, __PRETTY_FUNCTION__, __LINE__}, dumps_count);\
}

const size_t MAX_WORD_LEN = 50;

const int OP_NONE  = 0;
const int OP_ADD   = 1;
const int OP_SUB   = 2;
const int OP_MUL   = 3;
const int OP_DIV   = 4;
const int OP_POW   = 5;

enum IogBTNodeState {
  USUAL    = 0,
  STRESSED = 1,
};

enum IogBTNodeType {
  NUMBER    = 0,
  OPERATION = 1,
  VARIABLE  = 2,
};


struct IogDebugInfo_t {
  const char *name;
  const char *file_name;
  const char *func_name;
  int line_num;
};

struct IogBTNode_t {
  IogBTData_t  data;
  IogBTNode_t *right;
  IogBTNode_t *left;

  IogBTNodeState state;
  IogBTNodeType type;
};

IogBTNode_t *iog_BTNodeInit (IogBTData_t data, IogBTNodeType type = NUMBER);
IogBTNode_t *iog_BTNodeCopy (const IogBTNode_t *node);

IogBTNode_t *iog_BTCopy     (const IogBTNode_t *root);

int iog_BTNodeEnNull (IogBTNode_t *node);

int iog_BTNodeInsert  (IogBTNode_t *root, IogBTNode_t *node);
int iog_BTNodeInsertD (IogBTNode_t *root, IogBTData_t data);

int iog_BTNodeInitRight (IogBTNode_t *node, IogBTData_t data, IogBTNodeType type = NUMBER);
int iog_BTNodeInitLeft  (IogBTNode_t *node, IogBTData_t data, IogBTNodeType type = NUMBER);
int iog_BTNodeHangRight (IogBTNode_t *node, IogBTNode_t *hanging_node);
int iog_BTNodeHangLeft  (IogBTNode_t *node, IogBTNode_t *hanging_node);

int iog_BTDestroy (IogBTNode_t **root);

int iog_BTDump    (const IogBTNode_t *root, IogDebugInfo_t debug, size_t *dumps_count);

#endif // IOG_BIN_TREE_H
