#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 128

typedef enum { false, true } bool;

typedef struct {
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

	printf("converting %s\n", str);

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
#endif
