#include "mapread.h"

int *allocate_leafarr(int len){
	int i=0, *leaves = NULL;

	leaves = (int*)malloc(sizeof(int)*(len));

	for(i=0;i<len;i++){
		leaves[i] = -1;
	}

	return leaves;
}

void prepareST(Node *root){
	leafarray = allocate_leafarr(inputLen +1);

	DFS_PrepareST(root);
}

void DFS_PrepareST(Node *T){
	if(T==NULL) return;

	if (T->numChildren == 0){ // is a leaf node
		leafarr[nextIndex] = T->id;
		if(T->depth >= 1){
			T->start_index = nextIndex;
			T->end_index = nextIndex;
		}
		nextIndex++;
		return;
	} else {
		unsigned int i = 0;
		for(i=0; i < T->numChildren; i++){
			DFS_PrepareST(T->children[i]);
		}
		if(T->depth >= 1){
			T->start_index = T->children[0]->start_index;
			T->end_index = T->children[T->numChildren -1]->end_index;
		}
	}
}

Node *readNodeHop( Node *n, char *buff, unsigned int head, unsigned int tail){
	unsigned int numChild = 0, i = 0, x, y, min;
	Node *a = matchChild(n, buff, head, &numChild);
	// if there isnt a child that matches return that node
	if( a == NULL){
		//if ( strlen(beta) == 1 )
		//	return n;
		return (a);
	}
	//x = (int)strlen(beta);
	//y = (int)strlen(a->parentEdgeLabel);
	x = tail - head + 1;
	y = a->suffixTail - a->suffixHead + 1;
	min = y ^ ((x ^ y) & -(x < y));
	for( i = 0; i < min; i++){
		if( buff[head + i] != buff[a->suffixHead + i] ){
			return (n);
		}
	}
	// not an ending leaf and the for loop has gone through the string
	return (nodeHop( a, buff, head+i, tail));
}

int matchesBelow(Node *n, char *buff, int start){
	unsigned int i=0, j=0, count=0;
	Node *child = matchChild(n, buff, start, &i);
	for(j=0; j<child->suffixHead - child->suffixTail +1; j++){
		if(buff[start] == ibuff[child->suffixHead+j]){
			count++;
		} else {
			break;
		}
	}
	return count;
}

Node *findLoc(Node *root, char *r){
	Node *T = root;
	unsigned int read_ptr = 0, i = 0, mostMatches = 0;
	Node *deepestNode = NULL;
	while(T){
		Node *u = readNodeHop(T, r, 0, strlen(r)-1);
		read_ptr = u->depth;
		i = matchesBelow(u, r, u->depth);
		if(u->depth + i > mostMatches){
			deepestNode = u;
			mostMatches = u->depth + i;
		}
		T = u->suffixLink;
	}
	return deepestNode;
}

void mapReads(Node *root){
	unsigned int i;
	Node *deepest = NULL;
	for(i=0;i<numReads;i++){
		deepest = findLoc(root, readsList[i]);

	}
}
