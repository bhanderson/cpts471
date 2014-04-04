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


Node *makeNode( unsigned int id, Node *parent,
		char *parentEdgeLabel, unsigned int nodeDepth){
	Node *newnode = (Node *)malloc(sizeof(Node));
	newnode->id = id;
	newnode->suffixLink = newnode;
	newnode->parent = parent == NULL ? newnode : parent;
	if( parentEdgeLabel != NULL)
		newnode->parentEdgeLabel = malloc(strlen(parentEdgeLabel));

	strncpy(newnode->parentEdgeLabel, parentEdgeLabel, strlen(parentEdgeLabel));
	newnode->numChildren = 0;
	newnode->children = NULL;
	newnode->nodeDepth = nodeDepth;
	return newnode;
}


Node *suffixTree( char *input){
	Node *root = makeNode(0, NULL, NULL, 0);
	Node *currentNode = root;
	char *currentSuffix = input;
	unsigned int nodeId=1;
	unsigned int nodeDepth=1;
	int i;
	for( i=0; i<inputLen; i++ ){
		if( currentNode->numChildren == 0 ){
			currentNode->numChildren += 1;
			currentNode->children[0] = makeNode(nodeId,
					currentNode->parent,
					currentSuffix,
					nodeDepth);

		}
		currentSuffix++;
	}
	return root;
}


int printChildren( Node *n ){
	int i;
	for( i = 0; i < n->numChildren; i++ ){
		printf("%d", n->id);
	}
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
int bwt( char *input ){
	
	
	return (0);
}

