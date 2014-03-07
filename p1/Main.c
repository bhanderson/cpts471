#include "Header.h"

int main(int argc, const char *argv[])
{
	if(argc<1)
		return -1;
	if(settings(argv)){
		return -1;
	}
	//printf("S1 length: %d\nS2 length: %d\n", (int)strlen(s1), (int)strlen(s2));
	align(s1,s2);
//	printarray();
	if(local)
		localretrace(highscore[0],highscore[1]);
	else
		retrace();
	dynamicfree();

	return 0;
}
