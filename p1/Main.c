#include "Header.h"

int main(int argc, const char *argv[])
{
	char linein[256];
	char input;
	char one[MAXSTRLEN], two[MAXSTRLEN];

	if(settings(argv)){
		return -1;
	}
	while (1) {
		print_menu();
		int i,j;
		init();
		if(fgets(linein, sizeof(linein), stdin))
			input = linein[0];
		switch(input){
			case 'n':
			case 'N':
				printf("Needleman-Wunsch:\n");
				break;
			case 's':
			case 'S':
				printf("Smith-Waterman:\n");
				break;
			case 'q':
			case 'Q':
				return 0;
				break;
			default:
				printf("Error invalid command\n");
				break;
		}
	}
	return 0;
}
