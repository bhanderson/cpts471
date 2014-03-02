#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAXSTRLEN 12000

#define SUB 1
#define DEL 2
#define INS 4

int local = 0, ma = 0, mi = 0, g = 0, h = 0;
char s1[MAXSTRLEN], s2[MAXSTRLEN];
char *dynamicstring;
char s1name[64], s2name[64];
int highscore[2];


typedef struct DP_cell{
	int score;
	int dir; // S: 1 D: 3 I: 5	and any combination of the three
} DP_cell;


DP_cell **dynamicarray;
DP_cell array[MAXSTRLEN][MAXSTRLEN];


void print_menu(){
	printf("(N)eedleman-Wunsch,\n(S)mith-Waterman,\n(Q)uit\n");
}


void wordwrap(char* a1, char* m, char* a2){
	int count1 = 1, count2 = 1;
	int i;
	int apos = 0, bpos = 0, mpos = 0;
	int j= 0;
	while(a2[bpos] != 0 && a1[apos] != 0){
		i = 0;
		printf("s1: %d\t", count1);
		for (i = 0; i < 60 && a1[apos] != 0; i++) {
			printf("%c", a1[apos]);
			if (a1[apos] == 'a' || a1[apos] == 'c' || a1[apos] == 'g' || a1[apos] == 't' || a1[apos] == 'A' || a1[apos] == 'C' || a1[apos] == 'G' || a1[apos] == 'T') {
				count1++;
			}
			apos++;
		}
		printf(" %d", count1-1);
		printf("\n");
		printf("      \t");
		i = 0;
		for (i = 0; i < 60 && m[mpos] != 0; i++) {
			printf("%c", m[mpos]);
			mpos++;
		}
		printf("\n");

		printf("s2: %d\t", count2);
		i = 0;
		for (i = 0; i < 60 && a2[bpos] != 0; i++) {
			printf("%c", a2[bpos]);
			if (a2[bpos] == 'a' || a2[bpos] == 'c' || a2[bpos] == 'g' || a2[bpos] == 't' || a2[bpos] == 'A' || a2[bpos] == 'C' || a2[bpos] == 'G' || a2[bpos] == 'T') {
				count2++;
			}
			bpos++;
		}
		printf(" %d", count2-1);
		printf("\n");
		j++;
	}
	return;
}


char* revstring(char *str){
	int end = strlen(str)-1;
	int start = 0;

	while(start<end)
	{
		str[start] ^= str[end];
		str[end] ^= str[start];
		str[start]^= str[end];
		++start;
		--end;
	}
	return str;
}


void inputerror(){
	printf("Usage: <executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>\n");
}


int max(int s, int d, int i, int *dir){ // substitution deletion insertion
	if(s > d && s > i){
		*dir = SUB;
		return s;
	}
	if (d > s && d > i){
		*dir = DEL;
		return d;
	}
	if (i > d && i > s) {
		*dir = INS;
		return i;
	}
	if (s == d && s == i){
		*dir = SUB | DEL | INS;
		return s;
	}
	else if (s == d && s != i){
		*dir = SUB | DEL;
		return d;
	}
	else if (d == i && d != s){
		*dir = DEL | INS;
		return i;
	}
	else if (i == s && i != d){
		*dir = INS | SUB;
		return i;
	}
	return -1;
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


int align(char *s1, char *s2){
	int i,j,m,n, dir = 0;
	int su, de, in;
	m = strlen(s1);
	n = strlen(s2);
	array[0][0].score = 0;
	for (i = 1; i < MAXSTRLEN; i++) { // initialize the first column and row
		array[i][0].score = h + (g * (i-1));
		array[i][0].dir = DEL;
		if (local) {
			array[i][0].score = 0;
		}
	}
	for (j = 1; j < MAXSTRLEN; j++) {
		array[0][j].score = h + (g * (j-1));
		array[0][j].dir = INS;
		if (local) {
			array[0][j].score = 0;
		}
	}

	for (i = 1; i < m+1; i++) {
		for (j = 1; j < n+1; j++) {
			// substitution
			if (strncmp(&s1[i-1], &s2[j-1], 1)==0)
				su = array[i-1][j-1].score + ma;
			else
				su = array[i-1][j-1].score + mi;
			// deletion
			if(array[i-1][j].dir & DEL)
				de = array[i-1][j].score + g;
			else
				de = array[i-1][j].score + h + g;
			// insertion
			if(array[i][j-1].dir & INS)
				in = array[i][j-1].score + g;
			else
				in = array[i][j-1].score + h + g;

			// set the score and the direction the score was found to the cell
			array[i][j].score = max(su, de, in, &dir);
			if (array[i][j].score > array[highscore[0]][highscore[1]].score) {
				highscore[0] = i;
				highscore[1] = j;
			}
			if (local && array[i][j].score < 0) {
				array[i][j].score = 0;
			}
			array[i][j].dir = dir;
		}
	}
	if (local)
		printf("Optimal Score: %d\n", array[highscore[0]][highscore[1]].score);

	else
		printf("Optimal Score: %d\n", array[i-1][j-1].score);
	return 0;
}


int stringsinput(char *path){
	FILE *sfp = fopen(path, "r");
	int c, n = 0, strnum = 0;
	char line[256];

	if (sfp){
		while ((c = fgetc(sfp)) != EOF && n < MAXSTRLEN) {
			switch(c){
				case 'a':
				case 'A':
				case 'c':
				case 'C':
				case 't':
				case 'T':
				case 'g':
				case 'G':
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
				default:
					break;
			}
		}
		if(n == MAXSTRLEN){
			printf("ERROR SEQUENCE %d SIZE TOO LARGE\n", strnum);
			if(sfp)
				fclose(sfp);
			return -1;
		}
		printf("%s length %d\n", s1name, (int)strlen(s1));
		printf("%s length %d\n", s2name, (int)strlen(s2));
	}
	fclose(sfp);
	return 0;
}


int dynamicstrinput(char *path){
	struct stat st;
	char c, line[256];
	int size;
	char *pos;

	size = st.st_size;
	stat(path, &st);
	printf("FILESIZE: %d\n", size);
	dynamicstring = (char *)malloc(size * sizeof(char));
	pos = dynamicstring;
	FILE *dsfp = fopen(path, "r");
	if (!dsfp){
		return -1;
	} else {
		while((c = fgetc(dsfp)) != EOF){
			switch(c){
				case '>':
					fgets(line, sizeof(line), dsfp);
					char *tok = strtok(line, " ");
					strcpy(pos, tok);
					pos += strlen(tok);
					*pos = 0;
					pos++;
					break;
				case 'a':
				case 'A':
				case 'c':
				case 'C':
				case 'g':
				case 'G':
				case 't':
				case 'T':
						*pos = c;
						pos++;
				default:
					break;
			}

		}
	}

	return 0;
}


int settings(const char *argv[]){
	char sline[32];
	char temp;
	char strpath[64];
	char configpath[64];

	if(argv[1] && argv[2] && (*argv[2] == '0' || *argv[2] == '1')){
		strncpy(strpath, argv[1], 64);
		strncpy(&temp, argv[2], 1);
		local = temp - '0';
	}else{
		goto ERROR;
	}

	if(-1 == stringsinput(strpath)){
		return -1;
	}
	dynamicstrinput(strpath);

	if(argv[3]){
		strncpy(configpath, argv[3], 64);
	}

	printf("Local or Dynamic: ");
	if (!local) {
		printf("GLOBAL\n");
	} else if (local) {
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
//	if(fp)
//		fclose(fp);
	printf("TEST\n");
	inputerror();
	return -1;
}







int localretrace(int i, int j){ // input the position of where to start
	char revs1[MAXSTRLEN*2], revs2[MAXSTRLEN*2], match[MAXSTRLEN*2];
	revs1[0] = revs2[0] = match[0] = 0;
	int count = 0, dir=0;
	// while still in the array and not the last place on the alignment
	//printf("Start score: %d\n", array[i][j]);
	//printf("i: %d j: %d", i, j);
	//printf("S1: %s\n", s1);
	//printf("S2: %s\n", s2);
	while(array[i][j].score > 0){
		max(array[i-1][j-1].score, array[i-1][j].score, array[i][j-1].score, &dir);
		if(dir & SUB){ // substitution
			revs1[count] = s1[i-1];
			revs2[count] = s2[j-1];
			if (s1[i-1] == s2[j-1]) {
				strcat(match, "|");
			} else
				strcat(match, " ");
			i--; j--;
		} else if (dir & DEL) { // deletion
			revs1[count] = s1[i-1];
			revs2[count] = '-';
			strcat(match, " ");
			i--;
		} else if (dir & INS) {
			revs1[count] = '-';
			revs2[count] = s2[j-1];
			strcat(match, " ");
			j--;
		}
		count++;
	}
	//strcat(match, "?");
	revs1[count] = 0;
	revs2[count] = 0;
	//printf("%s\n", revstring(revs1));
	//printf("%s\n", revstring(match));
	//printf("%s\n", revstring(revs2));
	wordwrap(revs1, match, revs2);
	return 0;
}


int retrace(){
	int i = strlen(s1), j = strlen(s2);
	int dir = 0;
	int count = 0;
	int gaps = 0, matches = 0, mismatches = 0;
	char res1[MAXSTRLEN], res2[MAXSTRLEN], match[MAXSTRLEN];
	match[0]=0;

	while(i !=0 && j !=0){
		max(array[i-1][j-1].score, array[i-1][j].score, array[i][j-1].score, &dir);
		if(dir & SUB){
			res1[count] = s1[i-1];
			res2[count] = s2[j-1];
			if(s1[i-1]==s2[j-1]){
				matches++;
				strcat(match, "|");
			}
			else{
				mismatches++;
				strcat(match, " ");
			}
			i--;
			j--;
		} else if(dir & DEL){
			res1[count] = s1[i-1];
			res2[count] = '-';
			strcat(match, " ");
			gaps++;
			i--;
		}else if(dir & INS){
			res1[count] = '-';
			res2[count] = s2[j-1];
			strcat(match, " ");
			j--;
		}
		count++;
	}
	res1[count] = 0;
	res2[count] = 0;
	match[count] = 0;
	revstring(res1);
	revstring(match);
	revstring(res2);
	//printf("%s\n", res1);
	//printf("%s\n", res2);
	wordwrap(res1, match, res2);
	return 0;
}


void printarray(){
	int i,j;
	for (i = 0; i < (int)strlen(s1)+1; i++) {
		for (j = 0; j < (int)strlen(s2)+1; j++) {
			printf("%d\t", array[i][j].score);
		}
		printf("\n");
	}
	return;
}
#endif
