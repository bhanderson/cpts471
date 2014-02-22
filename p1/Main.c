#include "Header.h"

int main(int argc, const char *argv[])
{
	char linein[256];
	char input;

	if(settings(argv)){
		return -1;
	}
	while (1) {
		print_menu();
		if(fgets(linein, sizeof(linein), stdin))
			input = linein[0];
		switch(input){
			case 'n':
			case 'N':
				printf("Needleman-Wunsch:\n");
				align(s1,s2);
				//printf("%d", array[strlen(s1)][strlen(s2)]);
				//printarray();
				printf("SCORE: %d\n", array[strlen(s1)][strlen(s2)].score);
				retrace();
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
