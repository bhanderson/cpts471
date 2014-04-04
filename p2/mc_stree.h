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


/* ---------- DEFINITIONS ------- */

// classes and other defs go here...

typedef struct Node{		// ananth complete
	unsigned int id;	
	struct Node *suffixLink;
	struct Node *parent;
	char *parentEdgeLabel;	
	struct Node **children;
	int nodeDepth;
}Node;

/* ---------- PROTOTYPES -------- */
// function prototypes go here...
int suffixTree( char *input, char *alphabet );

int bfs( Node *root );

int dfs( Node *root );

int bwt( Node *root, char *input );

#endif
