#ifndef __HEADER_H__
#define	__HEADER_H__

void print_menu(){
	printf("Input string (1),\nInput string (2),\n(N)eedleman-Wunsch,\n(S)mith-Waterman,\n(Q)uit\n");
}

void seqin(char *c){
	printf("Enter the sequence: ");
	gets_s(c,64);
}
#endif