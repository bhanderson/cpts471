#include <stdio.h>
#include "Header.h"

int main(){
	char input;
	char one[64], two[64];
	print_menu();
	while(1){
		input = getchar();
		getchar();			// discard newline char
		switch(input){
		case '1':
			seqin(one);
			printf("%s\n",one);
			break;
		case '2':
			seqin(two);
			printf("%s\n",two);
			break;
		case 'n':
		case 'N':
			break;
		case 's':
		case 'S':
			break;
		case 'q':
		case 'Q':
			return 0;
			break;
		default:
			printf("Error invalid command\n");

		}
	}
}

