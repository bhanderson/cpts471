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
		char *parentEdgeLabel, unsigned int stringDepth){
	Node *newnode = (Node *)malloc(sizeof(Node));
	newnode->id = id;
	newnode->suffixLink = newnode;
	newnode->parent = parent == NULL ? newnode : parent;
	if( parentEdgeLabel != NULL)
		newnode->parentEdgeLabel = malloc(strlen(parentEdgeLabel));

	strncpy(newnode->parentEdgeLabel, parentEdgeLabel, strlen(parentEdgeLabel));
	newnode->numChildren = 0;
	newnode->children = NULL;
	newnode->depth = stringDepth;
	return newnode;
}

int findPath(Node *n, char *suffix){
	int i, j;
	char *current;
	bool mismatch = false;
	for( i=0; i < n->numChildren && !mismatch; i++){
		for( j=0; j < (int)strlen(n->children[i]->parentEdgeLabel) && !mismatch; j++ ){
			current = &n->children[i]->parentEdgeLabel[j];
			if( *current != suffix[j] )
				break;
			// else
			
		}
	}
	// cannot find a child that matches a 
	return (int)n;
}

int stringDepth(Node *u){
	Node *temp = u;
	int length = 0;
	while( temp->parent != NULL ){
		length+= strlen(temp->parentEdgeLabel);
		temp = temp->parent;
	}
	return length;
}


int identifyCase(Node *root, Node *u){
	if( u->suffixLink != NULL && u != root ) // case IA
		return 0;
	if( u->suffixLink != NULL && u == root ) // case IB
		return 1;
	if ( u->suffixLink == NULL && u->parent != root ) // case IIA
		return 2;
	if ( u->suffixLink == NULL && u->parent == root ) // case IIB
		return 3;
	return -1;
}


Node *insert( char *suffix, Node *root, Node *leaf ){
	Node *u = leaf->parent;
	int c = identifyCase( root, u );
	switch(c){
		case 0: // IA
		{
			unsigned int k = u->depth;
			u = u->suffixLink;
			findpath(u);

			break;
		}
		case 1:
		{

		}
		case 2:
		{

		}
		case 3:
		{

		}
		default:
			break;
	}
	return u;
}


Node *suffixTree( char *input ){
	Node *root = makeNode(0, NULL, NULL, 0);
	Node *leaf = root;
	int i;
	for( i=1; i <= inputLen; i++ ){
		leaf = insert( &input[i], root, leaf);
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
	printf("%d", node->depth);
	return 0;
}


/* Function: bwt()
 * Input:
 * 		*node: pointer to node in tree
 * Output:
 * 		int: returns 0 for success, -1 for failure
 * Summary: Burrows Wheeler Transform. Given an input string, construct a BWT
 */
int bwt( Node *node ){


	return (0);
}

