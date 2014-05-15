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
/*
Node *findLoc(Node *root, char *read){
	//Node *deepestNode = NULL, *u = NULL;
	Node *u = NULL;
	//unsigned int i = 0, read_ptr = 0, r = 0, mostMatches = 0;
	unsigned int i = 0;
	u = readNodeHop(root, read, i, strlen(read)-1);
	   for(i=0; i < strlen(read); i++){
	   read_ptr = u->depth;
	   r = matchesBelow(u, read, u->depth);
	   if(u->depth + r > mostMatches && u->depth>=1){
	   deepestNode = u;
	   mostMatches = u->depth + r;
	   }
	   }
	return u;
	   Node *T = root;
	   unsigned int read_ptr = 0, r = 0, mostMatches = 0;
	   Node *deepestNode = NULL;
	   do{
	   Node *u = readNodeHop(T, read, read_ptr, strlen(read)-1);
	   read_ptr = u->depth;
	   r = matchesBelow(u, read, u->depth);
	   if(u->depth + r > mostMatches && u->depth>=1){
	   deepestNode = u;
	   mostMatches = u->depth + r;
	   }
	   T = u->suffixLink;
	   } while (T!=root && read_ptr + r < strlen(read));
	   return deepestNode;
}
*/

Node *findPath(Node *n, char *buff){
	unsigned int buff_ptr = 0, ibuff_ptr = n->suffixHead;
	unsigned int i;
	unsigned int matchesBelow = 0;
	bool childFound = false;
	Node *current = n;
	while(buff_ptr < strlen(buff)){
		childFound = false;
		for(i=0;i < current->numChildren;i++){
			if(ibuff[current->children[i]->suffixHead] == buff[buff_ptr]){
				current = current->children[i];
				ibuff_ptr = current->suffixHead;
				childFound = true;
				matchesBelow = 0;
				break;
			}
		}
		if(!childFound)
			return current;

		while(ibuff_ptr <= current->suffixTail && buff_ptr < strlen(buff)){
			if(ibuff[ibuff_ptr] == buff[buff_ptr]){ // match
				ibuff_ptr ++;
				buff_ptr ++;
				matchesBelow +=1;
			} else { // mismatch
				buff_ptr -= matchesBelow;
				if(current->suffixLink == NULL)
					current = current->parent->suffixLink;
				else
					current = current->suffixLink;
				ibuff_ptr = current->suffixHead;
				break;
			}
		}
	}
	return current;
}

Node *findLoc2(Node *root, char *read){
	Node *T = root;
	//unsigned int read_ptr = 0;
	//Node *deepestNode = T;
	Node *u = findPath(T, read);
	/*
	while(read_ptr < strlen(read)){
		Node *u = readNodeHop(T, read, read_ptr, strlen(read)-1);
		read_ptr = u->depth+1;
		if(u->depth >=1 && u->depth > deepestNode->depth){
			deepestNode = u;
		}
		T = u->suffixLink;
	}
	return deepestNode;
	*/
	return u;
}

char *getSubstring(Node *n, int rlen, unsigned int *start, unsigned int *end){
	unsigned int i = 0;
	char *retval;
	if ((int)n->start_index - rlen < 0)
		*start = 0;
	else
		*start = n->start_index - rlen;

	if ((unsigned int)n->end_index + rlen > inputLen)
		*end = inputLen;
	else
		*end = n->end_index + rlen;

	retval = malloc(((*end - *start) + 1) * sizeof(char));
	if(!retval){
		perror("could not allocate substring");
		exit(-1);
	}
	for(i = *start; i < *end; i++){
		retval[i - *start] = ibuff[i];
	}
	retval[i- *start] = 0;
	return retval;
}

void mapReads(Node *root){
	unsigned int i = 0, readLen = 0, score = 0, start = 0, end = 0;
	unsigned int matches = 0, mismatches = 0, gaps = 0, openings = 0;
	unsigned int alignLen = 0;
	double percentIdentity = 0.0;
	double lenCoverage = 0.0;
	double maxCoverage = 0.0;
	unsigned int maxReadLocation;
	unsigned int bestHitStart = 0, bestHitEnd = 0;
	Node *deepest = NULL;
	char *subString;

	for(i=1; i<numReads*2; i+=2){
		readLen = strlen(readsList[i]);
		deepest = findLoc2(root, readsList[i]);
		subString = getSubstring(deepest, 25 /*readLen*/, &start, &end);

		MA = 1; MI = -2; H = -5; G = -1;

		//score = mapAlign(readsList[i], subString, &matches, &mismatches, &gaps, &openings);
		score = localAlign(readsList[i], subString, &matches, &mismatches, &gaps, &openings);
		dynamicfree(strlen(readsList[i]), strlen(subString));

		alignLen = matches + mismatches + gaps;
		percentIdentity = (double)matches / (double)alignLen;
		lenCoverage = (double)alignLen / (double)readLen;
		if(percentIdentity >= X && lenCoverage >= Y){
			if (lenCoverage > maxCoverage){
				maxCoverage = lenCoverage;
				maxReadLocation = i;
				//bestHitStart = leafarr[j] - readLen;
				bestHitStart = start;
				//bestHitEnd = leafarr[j] + readLen;
				bestHitEnd = end;
				printf("Read Name: %s\n", readsList[maxReadLocation-1]);
				printf("Start Index Hit: %d\n", bestHitStart);
				printf("End Index Hit: %d\n", bestHitEnd);
			} else {
				printf("<%s> no hit found\n", readsList[i-1]);
			}
		}
	} // end for i
	//	doNotBeLikeFirefox(root);
}

void printLabel(Node *n){
	unsigned int i=0;
	for(i= 0; i <= n->suffixTail; i++){
		printf("%c", ibuff[i]);
	}
	printf("\n");
}
