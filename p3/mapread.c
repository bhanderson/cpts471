#include "mapread.h"

int *allocate_leafarr(int len){
	int i=0, *leaves = NULL;

	leaves = (int*)malloc(sizeof(int)*(len));

	if(!leaves){
		perror("could not allocate leaves");
		exit(1);
	}

	for(i=0;i<len;i++){
		leaves[i] = -1;
	}

	return leaves;
}

void prepareST(Node *root){
	leafarr = allocate_leafarr(inputLen +1);

	DFS_PrepareST(root);
}

void DFS_PrepareST(Node *T){
	if(T==NULL) return;

	if (T->numChildren == 0){ // is a leaf node
		leafarr[nextIndex] = T->id - 1;
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

Node *readMatchChild( Node *n, char *buff, unsigned int suffix, unsigned int *i ){
	Node *current = NULL;
	//at node n check all children's first char
	//at the child
	for (*i = 0; *i < n->numChildren && n->numChildren > 0; *i+=1){
		current = n->children[*i];
		if (ibuff[current->suffixHead] == buff[suffix]){
			return (current);
		}
	}
	*i = -1;
	return (NULL);
}

Node *readNodeHop( Node *n, char *buff, unsigned int head, unsigned int tail){
	unsigned int numChild = 0, i = 0, x, y, min;
	Node *a = readMatchChild(n, buff, head, &numChild);
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
		if( buff[head + i] != ibuff[a->suffixHead + i] ){
			return (n);
		}
	}
	// not an ending leaf and the for loop has gone through the string
	return (readNodeHop( a, buff, head+i, tail));
}

int matchesBelow(Node *n, char *buff, int start){
	unsigned int i=0, j=0, count=0;
	Node *child = readMatchChild(n, buff, start, &i);
	if(child == NULL)
		return 0;
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
	do{
		Node *u = readNodeHop(T, r, 0, strlen(r)-1);
		read_ptr = u->depth;
		i = matchesBelow(u, r, u->depth);
		if(u->depth + i > mostMatches){
			deepestNode = u;
			mostMatches = u->depth + i;
		}
		T = u->suffixLink;
	} while (T!=root);
	return deepestNode;
}

char *getSubstring(int *length, int start, unsigned int end){
	if(start < 0)
		start = 0;
	if(end > inputLen)
		end = inputLen;

	*length = end - start + 1;

	return &ibuff[start];
}

void mapReads(Node *root){
	unsigned int i;
	int j, readLen=0, subLen, score;
	unsigned int matches = 0, mismatches = 0, gaps = 0, openings = 0;
	unsigned int alignLen = 0;
	double percentIdentity = 0.0;
	double lenCoverage = 0.0;
	double maxCoverage = 0.0;
	unsigned int bestHitStart = 0, bestHitEnd = 0;
	unsigned int maxReadLocation;
	Node *deepest = NULL;
	char *subString;
	for(i=1; i<numReads*2; i+=2){
		readLen = strlen(readsList[i]);
		deepest = findLoc(root, readsList[i]);
		for(j=deepest->start_index; j<= deepest->end_index; j++){
			subString = getSubstring(&subLen, leafarr[j] - readLen, leafarr[j] + readLen);
			MA = 1; MI = -2; H = -5; G = -1;
			score = localAlign(ibuff, subString, subLen, &matches, &mismatches, &gaps, &openings);
			alignLen = matches + mismatches + gaps;
			percentIdentity = (double)matches / (double)alignLen;
			lenCoverage = (double)alignLen / (double)readLen;
			if(percentIdentity >= X && lenCoverage >= Y){
				if (lenCoverage > maxCoverage){
					maxCoverage = lenCoverage;
					maxReadLocation = i;
					bestHitStart = leafarr[j] - readLen;
					bestHitEnd = leafarr[j] + readLen;
					printf("Read Name: %s\n", readsList[maxReadLocation-1]);
					printf("Start Index Hit: %d\n", bestHitStart);
					printf("End Index Hit: %d\n", bestHitEnd);
				} else {
					printf("<%s> no hit found\n", readsList[i-1]);
				}
			}
			dynamicfree(strlen(ibuff), strlen(subString)); // dptables are for loops
		} // end for j
	} // end for i
	//doNotBeLikeFirefox(root);
}
