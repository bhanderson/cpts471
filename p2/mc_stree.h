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

typedef int bool;
enum { false, true };		// get mad, this is worth it

typedef struct Node{		// ananth complete
	unsigned int id;	
	struct Node *suffixLink;
	struct Node *parent;
	char *parentEdgeLabel;	
	struct Node **children;
	int nodeDepth;
}Node;


/* ---------- PROTOTYPES -------- */

int suffixTree( char *input, char *alphabet );

int bfs( Node *root );

int dfs( Node *root );

int bwt( Node *root, char *input );

#endif	// _MC_STREE_H_
