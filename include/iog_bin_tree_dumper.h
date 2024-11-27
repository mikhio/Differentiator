#ifndef IOG_BIN_TREE_DUMPER_H
#define IOG_BIN_TREE_DUMPER_H

#include "iog_bin_tree.h"

#include <stdio.h>

const size_t MAX_DATETIME_STR_LEN = 100;
const size_t MAX_FILENAME_LEN     = 200;
const size_t MAX_CMD_STR_LEN      = 300;

const char DUMPS_FOLDER[MAX_FILENAME_LEN]      = "dumps";
const char GV_DUMPS_FOLDER[MAX_FILENAME_LEN]   = "gv_dumps";
const char PNG_DUMPS_FOLDER[MAX_FILENAME_LEN]  = "png_dumps";

int iog_BTGraphDump  (const IogBTNode_t *root, const IogDebugInfo_t debug, size_t *dumps_count);
int iog_BTGraphPrint (FILE *dump_file, const IogBTNode_t *root, int rank);

#endif // IOG_BIN_TREE_DUMPER_H
