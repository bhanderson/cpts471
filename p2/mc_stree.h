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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>   	// for getting file size


/* ---------- DEFINITIONS ------- */

int alphabetLen = 0;
int inputLen = 0;
int inodes = 0;

char *abuff;
char *ibuff;

typedef int bool;
enum { false, true };		// get mad, this is worth it


typedef struct Node{		// ananth complete
	unsigned int id;
	struct Node *suffixLink;
	struct Node *parent;
	char *parentEdgeLabel;
	int numChildren;
	struct Node **children;
	unsigned int depth;
}Node;


/* ---------- PROTOTYPES -------- */

Node *makeNode( unsigned int id, Node *parent,
				char *parentEdgeLabel, unsigned int Depth);

int findPath(Node *n, char *suffix);

int stringDepth(Node *u);

int identifyCase(Node *root, Node *u);

Node *insert( char *suffix, Node *root, Node *leaf);

Node *suffixTree( char *input);


int printChildren( Node *n );

int dfs( Node *node );

int bwt( char *input );

#endif	// _MC_STREE_H_
