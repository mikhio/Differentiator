#ifndef IOG_BIN_TREE_H
#define IOG_BIN_TREE_H

#include "iog_return_codes.h"

#include <stdlib.h>

typedef double IogBTData_t;

#define IOG_BT_DUMP(root, dumps_count) {\
  iog_BTDump(root, {#root, __FILE__, __PRETTY_FUNCTION__, __LINE__}, dumps_count);\
}

enum IogBTNodeState {
  USUAL    = 0,
  STRESSED = 1,
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
};

IogBTNode_t *iog_BTNodeInit (IogBTData_t data);

int iog_BTNodeInsert  (IogBTNode_t *root, IogBTNode_t *node);
int iog_BTNodeInsertD (IogBTNode_t *root, IogBTData_t data);

int iog_BTDestroy (IogBTNode_t **root);

int iog_BTDump    (const IogBTNode_t *root, IogDebugInfo_t debug, size_t *dumps_count);

#endif // IOG_BIN_TREE_H
