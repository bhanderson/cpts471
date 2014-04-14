/*
 * Author:      Bryce Handerson (bhanderson)
 *              Andrew M Bates (abates09 | andrewbates09)
 * File:        mc_stree.h
 * Project:     CptS 471 - Project 2
 * Due:         April 04, 2014
 *
 * Notes:
 * The goal of this project is to implement the McCreight's suffix tree con-
 *      struction algorithm, given an input string s (in the FASTA file format)
 *      and a file containing the input alphabet.
 */

#ifndef _MC_STREE_H_
#define _MC_STREE_H_


/* ---------- INCLUDES ---------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ---------- DEFINITIONS ------- */

int alphabetLen = 0;
int inputLen = 0;
int inodes = 0;
int leafs = 1;

char *abuff;
char *ibuff;
char iname[16];

typedef int bool;
enum { false, true };		// get mad, this is worth it


typedef struct Node{		// ananth complete
	unsigned int id;
	unsigned int depth;
	unsigned int numChildren;
	unsigned int suffixHead;
	unsigned int suffixTail;
	struct Node *parent;
	struct Node *suffixLink;
	struct Node **children;
}Node;


/* ---------- PROTOTYPES -------- */

int addChild( Node *parent, Node *child );

Node *makeNode( unsigned int id, Node *parent,
		unsigned int suffixHead, unsigned int suffixTail, unsigned int Depth );

Node *matchChild( Node *n, unsigned int suffix, unsigned int *i );

Node *splitEdge( Node *current, unsigned int suffix);

Node *findPath( Node *n, unsigned int suffix );

int stringDepth( Node *u );

int identifyCase( Node *root, Node *u );

Node *nodeHop( Node *n, char *beta );

Node *insert( int i, Node *root, Node *leaf );

Node *suffixTree( void );

int printChildrenLabels( Node *n );

int printChildren( Node *n );

int dfs( Node *node );

int bwt( Node *node );

void doNotBeLikeFirefox(Node *node);

#endif	// _MC_STREE_H_
