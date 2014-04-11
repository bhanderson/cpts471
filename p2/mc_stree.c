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

// constructor for creating a node
Node *makeNode( unsigned int id, Node *parent,
		char *parentEdgeLabel, unsigned int stringDepth ){
	Node *newnode = (Node *)malloc(sizeof(Node));
	newnode->id = id;
	newnode->suffixLink = NULL;
	newnode->parent = parent == NULL ? newnode : parent;
	if( parentEdgeLabel != NULL){
		newnode->parentEdgeLabel = calloc(1, sizeof(char) * strlen(parentEdgeLabel));
		strncpy(newnode->parentEdgeLabel, parentEdgeLabel, strlen(parentEdgeLabel));
	}
	newnode->numChildren = 0;
	newnode->children = malloc(alphabetLen * sizeof(Node *));
	newnode->depth = stringDepth;
	return newnode;
}


// find the child that matches the first character of the suffix
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

// split an edge to insert a node
// make this recursive yo
Node *splitEdge( Node *current, char *suffix, int *i ){
	int j, k;
	for(j=0; j < (int)strlen(suffix); j++){
		if ( current->parentEdgeLabel[j] == '\0'){ // >= (int)strlen(current->parentEdgeLabel) ){
			Node *temp = matchChild(current, &suffix[j], i);
			if (temp){
				return splitEdge(temp, &suffix[j], i);
			}
			Node *child = makeNode( (&suffix[j] - ibuff) + 1, current,
									&suffix[j], (strlen(&suffix[j]) + current->depth));
			addChild(current, child);
			return child;
		}
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

			char *childEdge = calloc(1, sizeof(char)*strlen(current->parentEdgeLabel) -j);
			memcpy(childEdge, &current->parentEdgeLabel[j], (strlen(current->parentEdgeLabel) - j));
			free(current->parentEdgeLabel);
			current->parentEdgeLabel = childEdge;

			Node *newLeaf = makeNode( (suffix - ibuff) + 1, newInode,
					&suffix[j], (strlen(&suffix[j]) + newInode->depth));
			addChild(newInode, newLeaf);
			return newLeaf;
		}
	}
	return NULL;
}

// find where to input suffix given a node
Node *findPath( Node *n, char *suffix ){
	int i = 0;
	Node *current = matchChild(n, suffix, &i);
	if ( current==NULL ){ // no child matches, make a new child leaf
		current = makeNode( (suffix - ibuff) +1, // pointer math for id
				n, suffix, (n->depth + strlen(suffix)));
		addChild(n, current);
	} else { // a child matches!! edge split....
		current = splitEdge(current, suffix, &i);
	}
	return current;
}

// what is the depth of the given node
int stringDepth( Node *u ){
	Node *temp = u;
	int length = 0;
	while( temp->parent != NULL ){
		length+= strlen(temp->parentEdgeLabel);
		temp = temp->parent;
	}
	return length;
}

// find the type of node to insert
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

// given a node and a suffix find the end of the suffix by traversing down
Node *nodeHop( Node *n, char *beta ){
	int numChild = 0, i = 0;
	Node *a = matchChild(n, beta, &numChild);
	// if there isnt a child that matches return that node
	if( a == NULL){
		return n;
	}
	for( i = 0; i < (int)strlen(beta) && i < (int)strlen(a->parentEdgeLabel); i++){
		if( beta[i] != a->parentEdgeLabel[i] ){
			return n;
		}
	}
	// not an ending leaf and the for loop has gone through the string
	return nodeHop( a, &beta[i]);
}

Node *ananthHop( Node *vPrime, Node *u, char *beta , int *i){
	Node *current = vPrime;
	Node *leaf = NULL;
	Node *child = NULL;
	char *e;
	char *b = beta;
	int r = 0, z = 0;
	while( r <= (int)strlen(b) ){
		z = 0;
		child = matchChild( current, &b[r], &z);
		if( child ){
			e = child->parentEdgeLabel;
			if( (strlen(e) + r) > strlen(b) ){
				leaf = splitEdge( child, &ibuff[*i+r], &z);
				Node *v = leaf->parent;
				u->suffixLink = v;
				return leaf;
			} else if( (strlen(e) + r) == strlen(b) ){
				Node *v = current;
				u->suffixLink = v;
				leaf = findPath(v, &ibuff[*i + u->depth - 1]);
				return leaf;
			} else {
				r = r + strlen(e);
				current = current->children[z];
			}
		} else
			return NULL;
	}
	return leaf;
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
				return findPath(u, &ibuff[i]);
				break;
			}
			// IIA suffix link for u is unknown and uprime is not the root
		case 2:
			{
				Node *uPrime = u->parent;
				//char beta[strlen(u->parentEdgeLabel)];
				//strncpy(beta, u->parentEdgeLabel, strlen(u->parentEdgeLabel));
				char *beta = u->parentEdgeLabel;
				Node *vPrime = uPrime->suffixLink;
				return ananthHop(vPrime, u, beta, &i);
				break;
			}
			// IIB suffix link for u is unknown and u' is the root
		case 3:
			{
				Node *v = NULL;
				Node *uPrime = u->parent;
				char *beta = u->parentEdgeLabel;
				char *betaPrime = &beta[1];

				if (strlen(betaPrime) == 0){
					u->suffixLink = uPrime;
					v = findPath(uPrime, &ibuff[i]);
				} else
					v = ananthHop(uPrime, u, betaPrime, &i);
				return v;
				break;
			}
		default:
			return findPath(root, &ibuff[i]);
			break;
	}
}


Node *suffixTree( void ){
	Node *root = makeNode(0, NULL, NULL, 0);
	root->suffixLink = root;
	Node *leaf = root;
	int i;
	for( i=0; i < inputLen; i++ ){
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


int dfs( Node *node ){
	printf("Depth: %d\t", node->depth);
	printf("NID: %d\t", node->id);
	if ( node != NULL ) {
		printf("Parent: %d\t", node->parent->id);
		printf("EdgeLabel: %s\t", node->parentEdgeLabel);
	}
	if( node->suffixLink )
		printf("SL->ID: %d\n", node->suffixLink->id); //, node->suffixLink);
	else
		printf("SL->ID: NULL\n");
	int i;
	for ( i = 0; i < node->numChildren; ++i)
	{
		dfs(node->children[i]);
	}
	return 0;
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
