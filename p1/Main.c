#include "Header.h"

int main(int argc, const char *argv[])
{
	if(argc<1)
		return -1;
	if(settings(argv)){
		return -1;
	}
	//printf("S1 length: %d\nS2 length: %d\n", (int)strlen(s1), (int)strlen(s2));
	//align(s1,s2);
	if (align(s1, s2))
		return -1;
	retrace();
//	printarray();
//	printf("%d ", threemax(-2175646, -7, -10));
	/*
	if(LOCAL)
		localretrace(highscore[0],highscore[1]);
	else
		retrace();
		*/
	dynamicfree();

	return 0;
}
