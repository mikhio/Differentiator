#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "iog_bin_tree.h"

int differentiate_tree (IogBTNode_t *root);

static int spreadDivision (IogBTNode_t *root);
static int convDivToPow (IogBTNode_t *root);

#endif // DIFFERENTIATOR_H
