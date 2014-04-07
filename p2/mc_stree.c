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
int addChild( Node *parent, Node *child){
	parent->children = realloc(parent->children, parent->numChildren +1 * (sizeof(Node *)));
	parent->children[parent->numChildren] = child;
	parent->numChildren++;
	// sort the children
	int i,j;
	Node *temp;
	for( i=0; i < parent->numChildren - 1; i++ ){
		for( j=0; j < parent->numChildren - 1 - i; j++ ){
			if( parent->children[j]->parentEdgeLabel[0] >
				parent->children[j+1]->parentEdgeLabel[0] ){
				temp = parent->children[j+1];
				parent->children[j+1] = parent->children[j];
				parent->children[j] = temp;
			}
		}
	}
	return 0;
}

Node *makeNode( unsigned int id, Node *parent,
		char *parentEdgeLabel, unsigned int stringDepth){
	Node *newnode = (Node *)malloc(sizeof(Node));
	newnode->id = id;
	newnode->suffixLink = NULL;
	newnode->parent = parent == NULL ? newnode : parent;
	if( parentEdgeLabel != NULL){
		newnode->parentEdgeLabel = malloc(strlen(parentEdgeLabel));
		strncpy(newnode->parentEdgeLabel, parentEdgeLabel, strlen(parentEdgeLabel));
	}
	newnode->numChildren = 0;
	newnode->children = malloc(sizeof(Node *) * alphabetLen);
	newnode->children = NULL;
	newnode->depth = stringDepth;
	return newnode;
}

int findPath(Node *n, char *suffix){
	int i, j, k;
	Node *current = NULL;
	//
	//at node current check all children's first char
	//at the child 

	for (i=0; i < n->numChildren; i++){
		current = n->children[i];
		if (!current)
			break;
		if (current->parentEdgeLabel[0] == suffix[0]){
			break;
		}
	}

	if ( current==NULL ){ // no child matches, make a new child leaf
		Node *current = makeNode( (suffix - ibuff) +1,
				n, suffix, (n->depth + strlen(&suffix[i])));
		addChild(n, current);
	} else { // a child matches!! edge split....
		for(j=0; j < (int)strlen(suffix); j++){
			if ( current->parentEdgeLabel[j] != suffix[j] ){
				// copy prefix of current node into temp array for internal node
				char prefix[j];
				for( k=0; k < j; k++){
					prefix[k] = suffix[k];
				}
				Node *newInode = makeNode( inputLen + inodes + 1,
						current->parent, prefix, current->parent->depth + j);
				inodes++;
				addChild(newInode, current);

				char *childEdge = malloc( sizeof(char)*strlen(current->parentEdgeLabel) -j);
				memcpy(childEdge, &current->parentEdgeLabel[j], (strlen(current->parentEdgeLabel) - j));
				free(current->parentEdgeLabel);
				current->parentEdgeLabel = childEdge;
				current->parent = newInode;

				Node *newLeaf = makeNode( (suffix - ibuff - 1), newInode,
						&suffix[j], (strlen(&suffix[j]) + newInode->depth));
				addChild(newInode, newLeaf);
			}
		}
	}
	/*
	   for ( i = 0; i < (int)strlen(suffix); i++ ){
	   for ( j = 0; j <= n->numChildren; j++ ){
	   current = &n->children[i]->parentEdgeLabel[j];
	   if ( n->children[j] == NULL ){ // create a new child
	   n->children[n->numChildren] = makeNode( (suffix - ibuff) + 1, // pointer math +1 because root is 0
	   n,
	   suffix,
	   (n->depth + strlen(&suffix[i])));
	   n->numChildren++;
	   return 0;
	   }
	   }
	   }


	   for( i=0; i < n->numChildren && !mismatch; i++){
	   for( j=0; j < (int)strlen(n->children[i]->parentEdgeLabel) && !mismatch; j++ ){
	   current = &n->children[i]->parentEdgeLabel[j];
	   if( *current != suffix[j] )
	   break;
// else

}
}
// cannot find a child that matches a 
*/
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
				//unsigned int k = u->depth;
				u = u->suffixLink;
				findPath(u, suffix);
				break;
			}
		case 1: // IB
			{
				findPath(u, suffix);
				break;
			}
		case 2: // IIA
			{
				Node *uPrime = u->parent;
				char beta[strlen(u->parentEdgeLabel)];
				strncpy(beta, u->parentEdgeLabel, strlen(u->parentEdgeLabel));
				Node *vPrime = uPrime->suffixLink;
				unsigned int k = vPrime->depth;
				// node hops to consume beta
				findPath(vPrime, &suffix[(k-1)]);
			}
		case 3: // IIB
			{
			//	Node *uPrime = u->parent;
			//	char beta[strlen(u->parentEdgeLabel)];
			//	strncpy(beta, u->parentEdgeLabel, strlen(u->parentEdgeLabel));
				//Node *vPrime = uPrime->suffixLink;

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

int printChildrenLabels( Node *n ){
	int i;
	if (n->parentEdgeLabel == NULL)
		printf("root's children: ");
	else
		printf("%s's children: ", n->parentEdgeLabel);
	for( i=0; i <n->numChildren; i++ ){
		printf("%s ", n->children[i]->parentEdgeLabel);
	}
	printf("\n");
	return 0;
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
 * 		*input: char poninter to beginning of string/array buffer
 * 		iLen: int length of input string
 * Output:
 * 		int: returns 0 for success, -1 for failure
 * Summary: Burrows Wheeler Transform. Given an input string, construct a BWT
 */
int bwt( char *input ){


	return (0);
}

