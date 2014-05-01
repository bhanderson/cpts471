#ifndef _MAPREAD_H_
#define _MAPREAD_H_

#include "stree.h"
#include "dptable.h"

int *leafarr = NULL;

void prepareST(Node *root);

void DFS_PrepareST(Node *T);

Node *readNodeHop( Node *n, char *buff, unsigned int head, unsigned int tail);

Node *findLoc(Node *root, char *r);

void mapReads(Node *root);

#endif
