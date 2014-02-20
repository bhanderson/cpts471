#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 256

int setting = 0, ma = 0, mi = 0, g = 0, h = 0;
char s1[MAXSTRLEN], s2[MAXSTRLEN];
char s1name[64], s2name[64];

typedef struct DP_cell{
	int score;
	char dir;
} DP_cell;


DP_cell array[MAXSTRLEN][MAXSTRLEN];

void print_menu(){
	printf("(N)eedleman-Wunsch,\n(S)mith-Waterman,\n(Q)uit\n");
}

void inputerror(){
	printf("Usage: <executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>\n");
}

int strtoint(char *str){
	int i = 0, val = 0, result = 0, temp = 0;

	//printf("converting %s\n", str);

	if(str[0] == '-'){
		for (i=1; str[i] <= '9' && str[i] >= '0'; i++) {
			val = str[i] - '0';
			temp = temp * 10 + val;
			result = temp * (-1);
		}
	} else{
		for (i = 0; str[i] <= '9' && str[i] >= '0'; i++) {
			val = str[i] - '0';
			temp = temp * 10 + val;
			result = temp;
		}
	}

	return result;
}

int align(char *s1, char *s2)
{

}

int init()
{
	int i;
	// set row and column zero to zero
	for (i = 0; i < MAXSTRLEN; i++) {
		array[0][i].score = 0;
		array[i][0].score = 0;
	}

}

int settings(const char *argv[]){
	char sline[32];
	char temp;
	char strpath[64];
	char configpath[64];

	if(argv[1] && argv[2] && (*argv[2] == '0' || *argv[2] == '1')){
		strncpy(strpath, argv[1], 64);
		strncpy(&temp, argv[2], 1);
		setting = temp - '0';
	}else{
		goto ERROR;
	}

	if(strinput(strpath)== -1){
		return -1;
	}

	if(argv[3]){
		strncpy(configpath, argv[3], 64);
	}

	printf("Local or Dynamic: ");
	if (setting == 0) {
		printf("GLOBAL\n");
	} else if (setting = 1) {
		printf("LOCAL\n");
	} else
		printf("UNKNOWN\n");
	//printf("Config: %s\n", configpath);

	FILE *fp = fopen(configpath, "r");

	if (fp) {
		while(fgets(sline, 32, fp)){
			char *tok = strtok(sline, " \t\n");
			if(strcmp(tok, "match") == 0){
				tok = strtok(NULL, "\t\n");
				//printf("%s",tok);
				ma = strtoint(tok);
				printf("Match score: %d\n", ma);
			}
			else if(strcmp(tok, "mismatch") == 0){
				tok = strtok(NULL, " \t\n");
				mi = strtoint(tok);
				printf("Mismatch score: %d\n", mi);
				//printf("%s\n", strtok(NULL, " \t\n"));

			}
			else if(strcmp(tok, "h") == 0){
				tok = strtok(NULL, " \t\n");
				h = strtoint(tok);
				printf("h score: %d\n", h);
			}
			else if(strcmp(tok, "g") == 0){
				tok = strtok(NULL, " \t\n");
				g = strtoint(tok);
				printf("g score: %d\n", g);
			}

		}
	}

	fclose(fp);
	return 0;

ERROR:
	if(fp)
		fclose(fp);
	printf("TEST\n");
	inputerror();
	return -1;
}

int strinput(char *path){
	//printf("strings: %s\n", strpath);
	FILE *sfp = fopen(path, "r");
	int c, n = 0, strnum = 0;
	char line[256];

	if (sfp){
		//printf("1\n");
		while ((c = fgetc(sfp)) != EOF && n < MAXSTRLEN) {
			switch(c){
				//printf("C: %c\n", c);
				case 'a':
				case 'c':
				case 't':
				case 'g':
					if(strnum == 1)
						s1[n++] = c;
					else
						s2[n++] = c;
					break;
				case '>':
					strnum++;
					n = 0;
					fgets(line, sizeof(line), sfp);
					char *tok = strtok(line, " ");
					if (strnum == 1) {
						strcpy(s1name, tok);
					} else if (strnum == 2) {
						strcpy(s2name, tok);
					}
					break;
			}
		}
		if(n == MAXSTRLEN){
			printf("ERROR SEQUENCE %d SIZE TOO LARGE\n", strnum);
			if(sfp)
				fclose(sfp);
			return -1;
		}
		printf("%s: %s\n", s1name, s1);
		printf("%s: %s\n", s2name, s2);
	}
	fclose(sfp);
	return 0;
}

#endif
