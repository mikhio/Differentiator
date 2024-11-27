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
    fprintf(dump_file,
      "elem_%p [\n"
      "  shape=record,\n"
      "  label=\"{elem_%p  | { {data | right | left} | {%lg | %p | %p} } }\",\n"
      "  fillcolor=%s,\n"
      "  color=black,\n"
      "  style=\"filled\"\n"
      "];\n",
      root,
      root, root->data, root->right, root->left,
      root->state == STRESSED ? "lightblue" : "lightgrey"
    );

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


