#include "Header.h"

int main(int argc, const char *argv[])
{
	if(argc<1)
		return -1;
	if(settings(argv)){
		return -1;
	}
	align(s1,s2);

	//	if(local)
	//		localretrace(highscore[0],highscore[1]);
	//	else
	//		retrace();
	dynamicfree();
	return 0;
}
