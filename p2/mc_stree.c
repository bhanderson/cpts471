/*
 * Author:      Bryce Handerson (bhanderson)
 *              Andrew M Bates (abates09 | andrewbates09)
 * File:        mc_stree.c
 * Project:     CptS 471 - Project 2
 * Due:         April 04, 2014
 * 
 * Notes:
 * The goal of this project is to implement the McCreight's suffix tree con-
 *      struction algorithm, given an input string s (in the FASTA file format)
 *      and a file containing the input alphabet.
 */


/* ---------- INCLUDES ---------- */

#include "mc_stree.h"


/* ---------- FUNCTION DEFS ----- */


int makeNode( ){
	return 0;
}

int suffixTree( char *input, char *alphabet ){
	
	return 0;
}


int dfs( Node *node ){
	int i;
	for( i=0; (i < node->numChildren) && (node->children[i] != NULL); i++ ){
		dfs( node->children[i] );
	}
	printf("%d", node->nodeDepth);
	return 0;
}

/* Function: bwt()
 * Input:
 * 		*input: char poninter to beginning of string/array buffer
 * 		iLen: int length of input string
 * Output:
 * 		int: returns 0 for success, -1 for failure
 * Summary: Burrows Wheeler Transform. Given an input string, construct a BWT
 */
int bwt( char *input, int inLen ){
	
	
	return (0);
}
