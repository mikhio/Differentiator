#include "iog_bin_tree_dumper.h"
#include "iog_bin_tree.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>
#include <time.h>
#include <string.h>


int iog_BTGraphDump (const IogBTNode_t *root, const IogDebugInfo_t debug, size_t *dumps_count) {
  IOG_ASSERT(dumps_count);

  char gv_filename[MAX_FILENAME_LEN] = "";
  char png_filename[MAX_FILENAME_LEN] = "";

  char datetime_str[MAX_DATETIME_STR_LEN] = "";

  time_t now = time(NULL);
  tm *t = localtime(&now);

  strftime(datetime_str, sizeof(datetime_str)-1, "%Y-%m-%d_%H.%M", t);

  snprintf(gv_filename, MAX_FILENAME_LEN, "./%s/%s/bin_tree_dump_%s-L%d-N%lu.gv",
      DUMPS_FOLDER,
      GV_DUMPS_FOLDER,
      datetime_str,
      debug.line_num,
      *dumps_count
  );

  snprintf(png_filename, MAX_FILENAME_LEN, "./%s/%s/bin_tree_dump_%s-L%d-N%lu.png",
      DUMPS_FOLDER,
      PNG_DUMPS_FOLDER,
      datetime_str,
      debug.line_num,
      *dumps_count
  );

  FILE *dump_file = fopen(gv_filename, "wb");

  if (dump_file == NULL) {
    fprintf(stderr, RED("DumpError: can't open dump file\n"));
    return ERR_CANT_OPEN_FILE;
  }
  
  fprintf(dump_file, "digraph G {\n");
  fprintf(dump_file,
      "debug_info [\n"
      "shape=record,\n"
      "label=\"{debug info | { {list name | file name | function name | line number | datetime} | {%s | %s | %s | %d | %s} }}\",\n"
      "fillcolor=white,\n"
      "color=grey,\n"
      "style=\"filled\",\n"
      "];\n",
      debug.name,
      debug.file_name,
      debug.func_name,
      debug.line_num,
      datetime_str
  );

  iog_BTGraphPrint(dump_file, root, 0);

  fprintf(dump_file, "}\n");

  fclose(dump_file);

  char dot_cmd[MAX_CMD_STR_LEN] = "";
  snprintf(dot_cmd, MAX_CMD_STR_LEN, "dot -Tpng %s -o %s", gv_filename, png_filename);

  system(dot_cmd);

  (*dumps_count)++;

  fprintf(stderr, GREEN("[DUMPED] ") BLACK("gv: %s\n"), gv_filename);
  fprintf(stderr,       "         "  BLACK("png: %s\n"), png_filename);

  return OK;
}

int iog_BTGraphPrint(FILE *dump_file, const IogBTNode_t *root, int rank) {
  IOG_ASSERT(dump_file);

  if (root != NULL) {
    
    printNode(dump_file, root);

    iog_BTGraphPrint(dump_file, root->left, rank+1);
    iog_BTGraphPrint(dump_file, root->right, rank+1);

    if (root->left != NULL)
      fprintf(dump_file, "elem_%p -> elem_%p;\n", root, root->left);
    if (root->right != NULL)
      fprintf(dump_file, "elem_%p -> elem_%p;\n", root, root->right);

    fprintf(dump_file, "\n");
  }

  return OK;
}

static int printNode(FILE *dump_file, const IogBTNode_t *node) {
  IOG_ASSERT(dump_file);
  IOG_ASSERT(node);

  char type_str[MAX_WORD_LEN] = "";
  char data_str[MAX_WORD_LEN] = "";

  switch ((int) node->type) {
    case NUMBER:
    {
      snprintf(type_str, MAX_WORD_LEN, "NUMBER");
      snprintf(data_str, MAX_WORD_LEN, "%lg", node->data);
      break;
    }
    case VARIABLE:
    {
      snprintf(type_str, MAX_WORD_LEN, "VARIABLE");
      snprintf(data_str, MAX_WORD_LEN, "%c", (char) node->data);
      break;
    }
    case OPERATION:
    {
      snprintf(type_str, MAX_WORD_LEN, "OPERATION");
      switch ((int) node->data) {
        case  OP_NONE:
          snprintf(data_str, MAX_WORD_LEN, "OP_NONE");
          break;
        case OP_ADD:
          snprintf(data_str, MAX_WORD_LEN, "OP_ADD");
          break;
        case OP_SUB:
          snprintf(data_str, MAX_WORD_LEN, "OP_SUB");
          break;
        case OP_MUL:
          snprintf(data_str, MAX_WORD_LEN, "OP_MUL");
          break;
        case OP_DIV:
          snprintf(data_str, MAX_WORD_LEN, "OP_DIV");
          break;
        case OP_POW:
          snprintf(data_str, MAX_WORD_LEN, "OP_POW");
          break;
        default:
          fprintf(stderr, RED("PrintError: Undefined operation type %lg of node %p\n"), node->data, node);
          snprintf(data_str, MAX_WORD_LEN, "UNDEFINED");
          break;
      }
      break;
    }
    default:
    {
      fprintf(stderr, RED("PrintError: Undefined type %d of node %p\n"), node->type, node);
      snprintf(type_str, MAX_WORD_LEN, "UNDEFINED"); 
      break;
    }
  }

  fprintf(dump_file,
    "elem_%p [\n"
    "  shape=record,\n"
    "  label=\"{elem_%p  | { {data | type | right | left} | {%s | %s | %p | %p} } }\",\n"
    "  fillcolor=%s,\n"
    "  color=black,\n"
    "  style=\"filled\"\n"
    "];\n",
    node,
    node, data_str, type_str, node->right, node->left,
    node->state == STRESSED ? "lightblue" : "lightgrey"
  );

  return OK;
}
