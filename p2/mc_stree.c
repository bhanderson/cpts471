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
// add a child to the given node and sort the children alphabetically
int addChild( Node *parent, Node *child ){
	//parent->children = realloc(parent->children, parent->numChildren +1 * (sizeof(Node *)));
	parent->children[parent->numChildren] = child;
	parent->numChildren++;
	child->parent = parent;
	// sort the children
	unsigned int i,j;
	Node *temp;
	for( i=0; i < parent->numChildren - 1; ++i ){
		for( j=0; j < parent->numChildren - 1 - i; ++j ){
			if( ibuff[parent->children[j]->suffixHead] >
					ibuff[parent->children[j+1]->suffixHead]) {
				/* parent->children[j]->parentEdgeLabel[0] >
					parent->children[j+1]->parentEdgeLabel[0] ){*/
				temp = parent->children[j+1];
				parent->children[j+1] = parent->children[j];
				parent->children[j] = temp;
			}
		}
	}
	return (0);
}

// constructor for creating a node
Node *makeNode( unsigned int id, Node *parent,
		unsigned int suffixHead, unsigned int suffixTail, unsigned int stringDepth ){
	Node *newnode = (Node *)malloc(sizeof(Node));
	if (newnode == NULL) {
		printf("\nERROR: could not malloc new node\n");
		exit (1);
	}
	newnode->id = id;
	newnode->suffixLink = NULL;
	newnode->parent = parent == NULL ? newnode : parent;
	newnode->suffixHead = suffixHead;
	newnode->suffixTail = suffixTail;
	/*if( parentEdgeLabel != NULL){
		//newnode->parentEdgeLabel = calloc(1, sizeof(char) * strlen(parentEdgeLabel));
		//strncpy(newnode->parentEdgeLabel, parentEdgeLabel, strlen(parentEdgeLabel));
		newnode->parentEdgeLabel = malloc(sizeof(char) * strlen(parentEdgeLabel));
		if (!newnode->parentEdgeLabel) {
			printf("\nERROR: could not malloc new node edge label\n");
			exit (1);
		}
		strcpy(newnode->parentEdgeLabel, parentEdgeLabel);
		newnode->parentEdgeLabel[strlen(parentEdgeLabel)] = '\0';
		
	}*/
	newnode->numChildren = 0;
	newnode->children = calloc(1, alphabetLen * sizeof(Node *));
	// newnode->depth = stringDepth;
	newnode->depth = stringDepth;
	return (newnode);
}


// find the child that matches the first character of the suffix
Node *matchChild( Node *n, unsigned int suffix, unsigned int *i ){
	Node *current = NULL;
	//at node n check all children's first char
	//at the child
	for (*i = 0; *i < n->numChildren && n->numChildren > 0; *i+=1){
		current = n->children[*i];
		if (ibuff[current->suffixHead] == ibuff[suffix]){
			return (current);
		}
	}
	return (NULL);
}

// split the current nodes parent edge with the suffix return the leaf
Node *splitEdge( Node *current, unsigned int head, unsigned int tail ){
	unsigned int i, min, x, y, z;
	matchChild(current->parent, head, &z);
	x = tail;
	y = current->suffixTail;
	min = y ^ ((x ^ y) & -(x < y));
	for( i = current->suffixHead; i <= tail; ++i ) {//(int)strlen(suffix); ++i ){
		if( ibuff[i] != ibuff[head + (i-current->suffixHead)] ){

			Node *newInode = makeNode( inputLen + inodes +1, current->parent,
					current->suffixHead, i - 1,
					current->parent->depth + i - current->suffixHead);
			inodes++;
			// need to set the current children to the new inodes children
			addChild(newInode, current);
			newInode->parent->children[z] = newInode;
			current->suffixHead = newInode->suffixTail + 1;
			//current->suffixTail = current->suffixTail;
			Node *newLeaf = makeNode( leafs,
					newInode, head + newInode->suffixTail - newInode->suffixHead + 1, tail,
					tail - (head + newInode->suffixTail - newInode->suffixHead) + newInode->depth);
			leafs ++;
			addChild(newInode, newLeaf);
			return (newLeaf);
		}
	}
	exit(1);
	return (current);
}

// find where to input suffix given a node
Node *findPath( Node *n, unsigned int head) { //}char *suffix ){
	unsigned int i = 0;
	unsigned int tail = inputLen -1;
	Node *hopped = nodeHop(n, head, tail);
	Node *current = matchChild(hopped, head+hopped->depth, &i);
	if ( current==NULL){ // no child matches, make a new child leaf
		current = makeNode( leafs,
				hopped, head + hopped->depth, tail, hopped->depth + (tail - head) +1);
		leafs++;
		addChild(hopped, current);
		return current;
	} else if( NULL/*tail - head == 1*/){
		head += current->depth;
		Node *leaf = makeNode( leafs,
				current, head, tail, current->depth + 1);
		leafs++;
		addChild(current, leaf);
		current = leaf;
	} else { // a child matches!! edge split....
		//current = splitEdge(current, &suffix[hopped->depth]);
		current = splitEdge(current, head + hopped->depth, tail);
	}
	return current;
}

// what is the depth of the given node
/*int stringDepth( Node *u ){
	Node *temp = u;
	int length = 0;
	while( temp->parent != NULL ){
		length+= strlen(temp->parentEdgeLabel);
		temp = temp->parent;
	}
	return length;
}
*/
// find the type of node to insert
int identifyCase( Node *root, Node *u ){
	if( u->suffixLink != NULL )
		if ( u != root )			// case IA
			return (0);
		else 						// case IB
			return (1);
	else if ( u->parent != root )	// case IIA
		return (2);
	else							// case IIB
		return (3);
	return (-1);
}

// given a node and a suffix find the end of the suffix by traversing down
// returns the parent that mismatches
Node *nodeHop( Node *n,unsigned int head, unsigned int tail){ 
	unsigned int numChild = 0, i = 0, x, y, min;
	Node *a = matchChild(n, head, &numChild);
	// if there isnt a child that matches return that node
	if( a == NULL){
		//if ( strlen(beta) == 1 )
		//	return n;
		return (n);
	}
	//x = (int)strlen(beta);
	//y = (int)strlen(a->parentEdgeLabel);
	x = tail - head + 1;
	y = a->suffixTail - a->suffixHead + 1;
	min = y ^ ((x ^ y) & -(x < y));
	for( i = 0; i < min; i++){
		if( ibuff[head + i] != ibuff[a->suffixHead + i] ){
			return (n);
		}
	}
	// not an ending leaf and the for loop has gone through the string
	return (nodeHop( a, head+i, tail));
}


Node *insert( unsigned int i, Node *root, Node *leaf ){
	// ananth is right we are wrong
	if (leaf == NULL){
		printf("ERROR Leaf returned null: i = %d",i);
		return (NULL);
	}
	Node *u = leaf->parent;
	int c = identifyCase( root, u );
	switch(c){
		// IA suffix link for u is known and u is not the root
		case 0:
			{
				int k = u->depth;
				Node *v = u->suffixLink;
				return (findPath(v, i));
				break;
			}
			// IB suffix link for u is known and u is the root
		case 1:
			{
				return findPath(u, i);
				break;
			}
			// IIA suffix link for u is unknown and uprime is not the root
		case 2:
			{
				//unsigned int j=0;
				Node *uPrime = u->parent;
				Node *vPrime = uPrime->suffixLink;
				unsigned int old = inodes;
				Node *n = findPath(vPrime, i);
				if (old <= inodes){
					u->suffixLink = n->parent;
				} else {
					u->suffixLink = uPrime;
				}
				return n;
				break;
			}
			// IIB suffix link for u is unknown and u' is the root
		case 3:
			{
				Node *uPrime = u->parent;
				Node *n;
				//unsigned betaPrime = &u->parentEdgeLabel[1];
				unsigned int betaPrimeLen = u->suffixTail - u->suffixHead;
				unsigned int old = inodes;
				n = findPath(uPrime, i);
				if (betaPrimeLen && (old <= inodes)){
					u->suffixLink = n->parent;
				} else {
					u->suffixLink = uPrime;
				}
				//if ( (u->suffixTail - u->suffixHead) && (old < inodes) ){
				//	u->suffixLink = n->parent;
				//} else {
				//	u->suffixLink = uPrime;
				//}
				return n;
				break;
			}
		default:
			return findPath(root, i);
			break;
	}
	return 0;
}


Node *suffixTree( void ){
	Node *root = makeNode(0, NULL, 0, 0, 0);
	root->suffixLink = root;
	Node *leaf = root;
	unsigned int i;
	for( i=0; i < inputLen; i++ ){
		//leaf = insert( &input[i], root, leaf);
		leaf = insert( i, root, leaf);
		if (leaf == NULL)
			return NULL;
	}
	return root;
}

/*
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
*/
/*
int printChildren( Node *n ){
	int i;
	for( i = 0; i < n->numChildren; i++ ){
		printf("%d", n->id);
	}
	return 0;
}
*/

// depth first search - preorder
int dfs( Node *node ){
	unsigned int i;
	printf("Depth: %d\t", node->depth);
	printf("NID: %d\t", node->id);
	if ( node != NULL ) {
		printf("Parent: %d\t", node->parent->id);
		//printf("EdgeLabel: %s\t", node->parentEdgeLabel);
		printf("EdgeLabel: ");
		for(i = node->suffixHead; i <= node->suffixTail; i++){
			printf("%c", ibuff[i]);
		}
		printf("\t");
	}
	if( node->suffixLink )
		printf("SL->ID: %d\n", node->suffixLink->id); //, node->suffixLink);
	else
		printf("SL->ID: NULL\n");
	for ( i = 0; i < node->numChildren; ++i)
	{
		dfs(node->children[i]);
	}
	return (0);
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
	unsigned int i;
	for( i=0; (i < node->numChildren) && (node->children[i] != NULL); i++ ){
		bwt( node->children[i] );
	}
	if (node->numChildren == 0)
	{
		int bwtval;
		bwtval = node->id - 1;
		printf("%c ", (bwtval > 0 ? ibuff[bwtval-1] : ibuff[inputLen - 1]));
	}
	return (0);
}

// free memory
void doNotBeLikeFirefox( Node *node ) {
	unsigned int i;
	if (node) { 
		for (i = 0; i < node->numChildren; ++i)
			doNotBeLikeFirefox(node->children[i]);
		if (node->numChildren == 0)
			free(node);
	}
}
