#ifndef _MAPREAD_H_
#define _MAPREAD_H_

#include "stree.h"
#include "dptable.h"

double X = .9;
double Y = .8;

int *leafarr = NULL;

unsigned int nextIndex = 0;

unsigned int readsLen = 0;

char *rbuff;

char **readsList;

unsigned int numReads = 0;

unsigned int maxReadIndex = 0;

void prepareST(Node *root);

void DFS_PrepareST(Node *T);

Node *readNodeHop( Node *n, char *buff, unsigned int head, unsigned int tail);

Node *findLoc(Node *root, char *r);

char *getSubstring(Node *n, int rlen, unsigned int *start, unsigned int *end);

void mapReads(Node *root);

void printLabel(Node *n);

#endif
