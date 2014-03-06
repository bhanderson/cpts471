#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define SUB 1
#define DEL 2
#define INS 4

int local = 0, ma = 0, mi = 0, g = 0, h = 0;
char *dynamicstring;
char *s1, *s2, *s1name, *s2name;
int highscore[2];
int strnum = 0;


typedef struct DP_cell{
	int score;
	int dir; // S: 1 D: 3 I: 5	and any combination of the three
} DP_cell;


DP_cell **dynamicarray;


void print_menu(){
	printf("(N)eedleman-Wunsch,\n(S)mith-Waterman,\n(Q)uit\n");
}


int dynamicfree(){
	int i = 0;
	if (dynamicarray){
		for (i=0;i< (int)strlen(s1)+1;i++){
			free(dynamicarray[i]);
		}
		free(dynamicarray);
	}
	if(dynamicstring)
		free(dynamicstring);
	return 0;
}


void wordwrap(char* a1, char* m, char* a2){
	int count1 = 1, count2 = 1;
	int i;
	int apos = 0, bpos = 0, mpos = 0;
	int j= 0;
	while(a2[bpos] != 0 && a1[apos] != 0){
		i = 0;
		printf("s1: %05d ", count1);
		for (i = 0; i < 60 && a1[apos] != 0; i++) {
			printf("%c", a1[apos]);
			if( a1[apos] != '-')
				count1++;
			apos++;
		}
		printf(" %d", count1-1);
		printf("\n");
		printf("\t  ");
		i = 0;
		for (i = 0; i < 60 && m[mpos] != 0; i++) {
			printf("%c", m[mpos]);
			mpos++;
		}
		printf("\n");

		printf("s2: %05d ", count2);
		i = 0;
		for (i = 0; i < 60 && a2[bpos] != 0; i++) {
			printf("%c", a2[bpos]);
			if( a2[bpos] != '-' )
				count2++;
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


int threemax(int a, int b, int c){
     int m = a;
     (m < b) && (m = b);
     (m < c) && (m = c);
     return m;
}


int maxscore(int i, int j, int *dir){
	int su = 0, de = 0, in = 0;
	int score = 0;
	// substitution
	if (strncmp(&s1[i-1], &s2[j-1], 1)==0)
		su = dynamicarray[i-1][j-1].score + ma;
	else
		su = dynamicarray[i-1][j-1].score + mi;
	// deletion
	if(dynamicarray[i-1][j].dir & DEL)
		de = dynamicarray[i-1][j].score + g;
	else
		de = dynamicarray[i-1][j].score + h + g;
	// insertion
	if(dynamicarray[i][j-1].dir & INS)
		in = dynamicarray[i][j-1].score + g;
	else
		in = dynamicarray[i][j-1].score + h + g;

	score = threemax(su, de, in);
	if (score == su)
		*dir |= SUB;
	if (score == de)
		*dir |= DEL;
	if (score == in)
		*dir |= INS;
	return score;
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


int allarr(int m, int n){
	DP_cell **temp;
	int i = 0;
	temp = malloc(m * sizeof(DP_cell*));
	if (temp == 0){
		printf("Error allocatign dynamic table");
		return -1;
	}
	for (i = 0; i < m; i++){
		temp[i] = malloc(n * sizeof(DP_cell));
		if(temp[i] == NULL){
			printf("Error allocating dynamic table");
			return -1;
		}
	}
	dynamicarray = temp;
	return 0;
}


int align(char *s1, char *s2){
	int i,j,m,n, dir = 0;
	int su, de, in;
	m = strlen(s1);
	n = strlen(s2);
	if(allarr(m+1,n+1)<0)
		return -1;
	dynamicarray[0][0].score = 0;
	for (i = 1; i < m+1; i++) { // initialize the first column and row
		dynamicarray[i][0].dir = DEL;
		if (local) {
			dynamicarray[i][0].score = 0;
		} else
			dynamicarray[i][0].score = h + (g * (i));
	}
	for (j = 1; j < n+1; j++) {
		dynamicarray[0][j].dir = INS;
		if (local) {
			dynamicarray[0][j].score = 0;
		} else
			dynamicarray[0][j].score = h + (g * (j));
	}

	for (i = 1; i < m+1; i++) {
		for (j = 1; j < n+1; j++) {
			// substitution
			if (strncmp(&s1[i-1], &s2[j-1], 1)==0)
				su = dynamicarray[i-1][j-1].score + ma;
			else
				su = dynamicarray[i-1][j-1].score + mi;
			// deletion
			if(dynamicarray[i-1][j].dir & DEL)
				de = dynamicarray[i-1][j].score + g;
			else
				de = dynamicarray[i-1][j].score + h + g;
			// insertion
			if(dynamicarray[i][j-1].dir & INS)
				in = dynamicarray[i][j-1].score + g;
			else
				in = dynamicarray[i][j-1].score + h + g;

			// set the score and the direction the score was found to the cell
			if (local && dynamicarray[i][j].score < 0) {
				dynamicarray[i][j].score = 0;
			} else
				dynamicarray[i][j].score = max(su, de, in, &dir);

			if (dynamicarray[i][j].score > dynamicarray[highscore[0]][highscore[1]].score) {
				highscore[0] = i;
				highscore[1] = j;
			}
			dynamicarray[i][j].dir = dir;
		}
	}
	if (local)
		printf("Optimal Score: %d\n", dynamicarray[highscore[0]][highscore[1]].score);

	else
		printf("Optimal Score: %d\n", dynamicarray[i-1][j-1].score);
	return 0;
}


int dynamicstrinput(char *path){
	struct stat st;
	char c, line[256];
	int size;
	char *pos;

	if (stat(path, &st) < 0){
		printf("Error reading file\n");
		return -1;
	}
	size = st.st_size;
	printf("Path: %s\n", path);
	printf("FILESIZE: %d\n", size);
	dynamicstring = (char *)malloc(size * sizeof(char));

	if (dynamicstring == 0)
		return -1;

	pos = dynamicstring;
	FILE *dsfp = fopen(path, "r");
	if (!dsfp){
		return -1;
	} else {
		while((c = fgetc(dsfp)) != EOF){
			switch(c){
				case '>':
					if (strnum == 0) // it is the first string
						s1name = pos;
					else{
						*pos = 0;
						pos ++;
						s2name = pos;
					}
					// get the line of the header
					// then copy the line to the dynamic array
					// terminate it with zero then move the pointer to the next
					// char position
					fgets(line, sizeof(line), dsfp);
					char *tok = strtok(line, " ");
					strcpy(pos, tok);
					pos += strlen(tok);
					*pos = 0;
					pos++;
					if (strnum == 0){
						s1 = pos;
					} else
						s2 = pos;
					strnum++;
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
					break;
				default:
					break;
			} // end switch

		}// end while
		*pos = 0;
	}
	if(dsfp)
		fclose(dsfp);
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

	if (dynamicstrinput(strpath) < 0){
		return -1;
	}

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
	int k = strlen(s1) + strlen(s2);
	char revs1[k], revs2[k], match[k];
	revs1[0] = revs2[0] = match[0] = 0;
	int count = 0, dir=0;

	while(dynamicarray[i][j].score > 0){
		max(dynamicarray[i-1][j-1].score, dynamicarray[i-1][j].score, dynamicarray[i][j-1].score, &dir);
		dir = dynamicarray[i][j].dir;
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
	revs1[count] = 0;
	revs2[count] = 0;
	wordwrap(revs1, match, revs2);
	return 0;
}


int dynamicretrace(){
	int i = strlen(s1), j = strlen(s2), k = i + j;
	int dir = 0, pos = 0, matches=0, mismatches=0, openings=0, gaps=0;
	int lastgap = 0;
	char res1[k], res2[k], match[k];
	while(i > 0 && j > 0){
		dir = dynamicarray[i][j].dir;
		if (dir & DEL){
			i--;
			res1[pos] = s1[i];
			res2[pos] = '-';
			match[pos] = ' ';
			if(!lastgap){
				openings++;
			}
			gaps++;
			lastgap = 1;
		} else if (dir & INS){
			j--;
			res1[pos] = '-';
			res2[pos] = s2[j];
			match[pos] = ' ';
			if(!lastgap){
				openings++;
			}
			gaps++;
			lastgap = 1;
		} else if (dir & SUB){
			i--;
			j--;
			res1[pos] = s1[i];
			res2[pos] = s2[j];
			if (s1[i] == s2[j]){
				matches++;
				match[pos] = '|';
			} else {
				mismatches++;
				match[pos] = ' ';
			}
			lastgap = 0;
		}
		pos++;
	}
	res1[pos] = 0;
	res2[pos] = 0;
	match[pos] = 0;
	revstring(res1);
	revstring(res2);
	revstring(match);
	wordwrap(res1, match, res2);
	printf("Matches: %d Mismatches: %d Openings: %d Gaps: %d\n",
			matches,	mismatches,	   openings,    gaps);
	return 0;
}


void printarray(){
	int i,j;
	for (i = 0; i < (int)strlen(s1)+1; i++) {
		for (j = 0; j < (int)strlen(s2)+1; j++) {
			printf("%d\t", dynamicarray[i][j].score);
		}
		printf("\n");
	}
	return;
}

#endif
