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
int addChild( Node *parent, Node *child ){
	parent->children = realloc(parent->children, parent->numChildren +1 * (sizeof(Node *)));
	parent->children[parent->numChildren] = child;
	parent->numChildren++;
	child->parent = parent;
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
		char *parentEdgeLabel, unsigned int stringDepth ){
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


Node *matchChild( Node *n, char *suffix, int *i ){
	Node *current = NULL;
	//at node n check all children's first char
	//at the child 
	for (; *i < n->numChildren && n->numChildren > 0; *i+=1){
		current = n->children[*i];
		if (current->parentEdgeLabel[0] == suffix[0]){
			return current;
		}
	}
	return NULL;
}


Node *splitEdge( Node *current, char *suffix, int *i ){
	int j, k;
	for(j=0; j < (int)strlen(suffix); j++){
		if ( current->parentEdgeLabel[j] != suffix[j] ){
			// copy prefix of current node into temp array for internal node
			char prefix[j+1];
			for( k=0; k < j; k++){
				prefix[k] = suffix[k];
			}
			prefix[k]=0;
			Node *newInode = makeNode( inputLen + inodes + 1,
					current->parent, prefix, current->parent->depth + j);
			inodes++;
			addChild(newInode, current);
			newInode->parent->children[*i] = newInode;

			char *childEdge = malloc( sizeof(char)*strlen(current->parentEdgeLabel) -j);
			memcpy(childEdge, &current->parentEdgeLabel[j], (strlen(current->parentEdgeLabel) - j));
			free(current->parentEdgeLabel);
			current->parentEdgeLabel = childEdge;

			Node *newLeaf = makeNode( (suffix - ibuff - 1), newInode,
					&suffix[j], (strlen(&suffix[j]) + newInode->depth));
			addChild(newInode, newLeaf);
			return newLeaf;
		}
	}
	return NULL;
}


Node *findPath( Node *n, char *suffix ){
	int i = 0;
	Node *current = matchChild(n, suffix, &i);
	if ( current==NULL ){ // no child matches, make a new child leaf
		Node *current = makeNode( (suffix - ibuff) +1, // pointer math for id
				n, suffix, (n->depth + strlen(&suffix[i])));
		addChild(n, current);
	} else { // a child matches!! edge split....
		current = splitEdge(current, suffix, &i);
	}
	return current;
}


int stringDepth( Node *u ){
	Node *temp = u;
	int length = 0;
	while( temp->parent != NULL ){
		length+= strlen(temp->parentEdgeLabel);
		temp = temp->parent;
	}
	return length;
}


int identifyCase( Node *root, Node *u ){
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


Node *insert( int i, Node *root, Node *leaf ){
	Node *u = leaf->parent;
	int c = identifyCase( root, u );
	switch(c){
		// IA suffix link for u is known and u is not the root
		case 0:
			{
				//unsigned int k = u->depth;
				Node *v = u->suffixLink;
				unsigned int k = u->depth;
				return findPath( v, &ibuff[i+k-1] );
				break;
			}
			// IB suffix link for u is known and u is the root
		case 1:
			{
				findPath(u, &ibuff[i]);
				break;
			}
			// IIA suffix link for u is unknown and uprime is not the root
		case 2:
			{/*
				Node *uPrime = u->parent;
				char beta[strlen(u->parentEdgeLabel)];
				strncpy(beta, u->parentEdgeLabel, strlen(u->parentEdgeLabel));
				Node *vPrime = uPrime->suffixLink;
				unsigned int k = vPrime->depth;
				// node hops to consume beta
				findPath(vPrime, &ibuff[(k-1)]);
				*/
			}
			// IIB suffix link for u is unknown and u' is the root
		case 3:
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


Node *suffixTree( void ){
	Node *root = makeNode(0, NULL, NULL, 0);
	root->suffixLink = root;
	Node *leaf = root;
	int i;
	for( i=1; i <= inputLen; i++ ){
		//leaf = insert( &input[i], root, leaf);
		leaf = insert( i, root, leaf);
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


/* Function: bwt()
 * Input:
 * 		*node: pointer to node in tree
 * Output:
 * 		int: returns 0 for success, -1 for failure
 * Summary: Depth First Search (preorder). Enumerate  nodes using DFS traversal.
 * 		This is a top down style. Print 
 */
int dfs( Node *node ){
	printf("Depth: %d\t", node->depth);
	printf("NID: %d\t", node->id);
	
	if ( parent != NULL ) {
		printf("Parent: %d\t", node->parent->id);
	}
	
	printf("SL: nope\n"); //, node->suffixLink);
	
	int i;
	for ( i = 0; i < numChildren; ++i;)
	{
		return ( dfs(children[i]) );
	}
	
}


/* Function: bwt()
 * Input:
 * 		*node: pointer to node in tree
 * Output:
 * 		int: returns 0 for success, -1 for failure
 * Summary: Burrows Wheeler Transform. Given an input string, construct a BWT
 * 		and print it out.  Basically, go to leafs and find representative val.
 */
int bwt( Node *node ){
	int i;
	for( i=0; (i < node->numChildren) && (node->children[i] != NULL); i++ ){
		dfs( node->children[i] );
	}
	if (node->numChildren == 0)
	{
		int bwtval;
		bwtval = inputLen - node->id;
		printf("%c", (bwtval > 0 ? ibuff[bwtval - 1] : ibuff[inputLen - 1]));
	}
	return (0);
}
