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
	unsigned int i=0,j=0;
	parent->children[parent->numChildren] = child;
	parent->numChildren++;
	child->parent = parent;
	// sort the children
	Node *temp;
	for( i=0; i < parent->numChildren - 1; ++i ){
		for( j=0; j < parent->numChildren - 1 - i; ++j ){
			if( ibuff[parent->children[j]->suffixHead] >
					ibuff[parent->children[j+1]->suffixHead]) {
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
	newnode->numChildren = 0;
	newnode->children = calloc(1, alphabetLen * sizeof(Node *));
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
	unsigned int i = 0, min = 0, x = 0, y = 0, z = 0;
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
	perror("splitEdge\n");
	exit(1);
	return (current);
}

Node *ananthFindPath( Node *v, unsigned int head ){
	unsigned int childNum, tail = inputLen -1;
	Node *hopped = nodeHop(v, head, inputLen -1);
	unsigned int hops = hopped->depth - v->depth;
	Node *child = matchChild(hopped, head+hops, &childNum);
	if ( child == NULL){
		child = makeNode(leafs, hopped,
				head + hops, tail,
				hopped->depth + (tail - head)+1);
		addChild(hopped, child);
		leafs++;
	} else { // a child exists
		child = splitEdge(child, head + hopped->depth, tail);
	}
	return child;
}

Node *ananthNodeHops(Node *vPrime, Node *u, unsigned int bHead,
		unsigned int bEnd, unsigned int suffix){
	unsigned int r = 0, childNum = 0, bLen = (bEnd - bHead + 1);
	Node *currNode = vPrime;
	Node *e = NULL, *i = NULL, *v = NULL;

	while(r <= (bLen)){ // r <= beta len
		// let e be the edge under currnode that starts with the character b[r]
		e = matchChild(currNode, bHead+r, &childNum);
		if(e){ // if e exists
			unsigned int edgeLen = (e->suffixTail - e->suffixHead + 1);
			if( edgeLen+r > bLen ){
				i = splitEdge(e, suffix, inputLen -1);
				v = i->parent;
				u->suffixLink = v;
				return i;
			} else if( edgeLen+r == bLen ){
				v = e;
				u->suffixLink = v;
				unsigned int k = u->depth;
				i = ananthFindPath(v, suffix + k -1);
				return i;
			} else {
				r += edgeLen;
				currNode = e;
				continue;
			}
		} else {
			i = makeNode(leafs, currNode, bHead, bEnd, currNode->depth + bLen);
			addChild(currNode, i);
			leafs++;
			v = i->parent;
			u->suffixLink = v;
			return i;
		}
	}
	return i;
}


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
				unsigned int k = u->depth;
				Node *v = u->suffixLink;
				//if (v->id == 0){
				//	return ananthFindPath(v, i);
				//} else {
					return ananthFindPath(v, i + k-1);
				//}
				break;
			}
			// IB suffix link for u is known and u is the root
		case 1:
			{
				return ananthFindPath(u, i);
				break;
			}
			// IIA suffix link for u is unknown and uprime is not the root
		case 2:
			{
				//unsigned int j=0;
				Node *uPrime = u->parent;
				unsigned int bHead = u->suffixHead, bTail = u->suffixTail;
				Node *vPrime = uPrime->suffixLink;
				return ananthNodeHops(vPrime, u, bHead, bTail, i);
				break;
			}
			// IIB suffix link for u is unknown and u' is the root
		case 3:
			{
				Node *uPrime = u->parent;
				unsigned int bHead = u->suffixHead, bTail = u->suffixTail;
				unsigned int bPrimeHead = bHead+1;
				if ((bTail - bPrimeHead + 1) == 0){
					u->suffixLink = uPrime;
					return ananthFindPath(uPrime, i);
				} else{
					return ananthNodeHops(uPrime, u, bPrimeHead, bTail, i);
				}
				//if (bPrimeHead > bTail){
				//	bPrimeHead = i;
				//	bTail = inputLen - 1;
				//}
				break;
			}
		default:
			perror("Exiting in insert\n");
			exit(1);
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
	unsigned int i;//,j;
	if (node){
		for(i=0; i < node->numChildren; i++){
			doNotBeLikeFirefox(node->children[i]);
		}
		//for(j=0;j<alphabetLen;j++)
		//	free(node->children[j]);
		//	node->children[j] = NULL;
		free(node->children);
		free(node);
	}
}
