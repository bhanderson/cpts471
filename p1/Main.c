#include "Header.h"

int main(int argc, const char *argv[])
{
	char input;
	char one[MAXSTRLEN], two[MAXSTRLEN];
	char strpath[64], configpath[64];
	char temp[1];
	bool setting;
	char line[32];
	int ma, mi, g, h;

	if(argv[1] && argv[2] && (*argv[2] == '0' || *argv[2] == '1')){
		strncpy(strpath, argv[1], 64);
		strncpy(temp, argv[2], 1);
		setting = *temp - '0';
	}else{
		inputerror();
		return 0;
	}

	if(argv[3]){
		strncpy(configpath, argv[3], 64);
	}

	printf("strings: %s\n", strpath);
	printf("Local or Dynamic: %d\n", setting);
	printf("Config: %s\n", configpath);

	FILE *fp;
	fp = fopen(configpath, "r");

	if (fp) {
		while(fgets(line, 32, fp)){
			char *tok = strtok(line, " \t\n");
			if(strcmp(tok, "match") == 0){
				tok = strtok(NULL, "\t\n");
				printf("%s",tok);
				ma = strtoint(tok);
				printf("Match score: %d\n", ma);
			}
			else if(strcmp(tok, "mismatch") == 0){
				tok = strtok(NULL, " \t\n");
				printf("Mismatch score: %d\n", strtoint(tok));
				//printf("%s\n", strtok(NULL, " \t\n"));

			}
			else if(strcmp(tok, "h") == 0){

			}
			else if(strcmp(tok, "g") == 0){

			}
			
		}
		/*
		   while(fgets(line, 32, fp)){
		   char *tok = strtok(line, " \t\n");
		   while(tok!= NULL){
		   printf("%s",tok);
		   tok = (NULL, " \t\n");
		   }
		   printf("a\n");
		   fgets(line, 32, fp);
		   }
		   */
	}

	while (1) {
		print_menu();
		int i,j;
		init();
		input = getchar();
		getchar(); // discard newline char
		switch(input){
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

	return 0;
}
