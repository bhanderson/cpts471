#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define SUB 1
#define DEL 2
#define INS 4

int LOCAL = 0, MA = 0, MI = 0, G = 0, H = 0;
char *dynamicstring;
char *s1, *s2, *s1name, *s2name;
int highscore[2];
int strnum = 0;


typedef struct DP_cell{
	int s;
	int d;
	int i;
	int dir; // S: 1 D: 2 I: 4	and any combination of the three
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
	if (b > m)
		m = b;
	if (c > m)
		m = c;
	return m;
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


int optimal(int i, int j){
	DP_cell *current = &dynamicarray[i][j];
	int max = current->s;
	if (max < current->d)
		max = current->d;
	if (max < current->i)
		max = current->i;
	return max;
}


int score_array(char *s1, char *s2){
	int i, j, m, n;
	m = strlen(s1);
	n = strlen(s2);

	// allocate the dynamic array
	if (allarr(m+1, n+1)<0)
		return -1;

	dynamicarray[0][0].s = 0;
	dynamicarray[0][0].d = 0;
	dynamicarray[0][0].i = 0;

	for (i=1; i <= m; i++){
		if (LOCAL){
			dynamicarray[i][0].s = 0;
			dynamicarray[i][0].d = 0;
			dynamicarray[i][0].i = 0;
		} else {
			dynamicarray[i][0].s = -2000000000;
			dynamicarray[i][0].d = H + (G * (i));
			dynamicarray[i][0].i = -2000000000;
		}
	}

	for (j=1; j <= n; j++){
		if (LOCAL){
			dynamicarray[0][j].s = 0;
			dynamicarray[0][j].d = 0;
			dynamicarray[0][j].i = 0;
		} else {
			dynamicarray[0][j].s = -2000000000;
			dynamicarray[0][j].d = -2000000000;
			dynamicarray[0][j].i = H + (G * (j));
		}
	}

	for (i = 1; i <= m; i++){
		for (j = 1; j <= n; j++){
			// match or mismatch
			if (s1[i-1] == s2[j-1])
				dynamicarray[i][j].s = optimal(i-1,j-1) + MA;
			else
				dynamicarray[i][j].s = optimal(i-1,j-1) + MI;
			// deletion
			dynamicarray[i][j].d = threemax(
					dynamicarray[i-1][j].d + G,
					dynamicarray[i-1][j].s + G + H,
					dynamicarray[i-1][j].i + G + H);
			// insertion
			dynamicarray[i][j].i = threemax(
					dynamicarray[i][j-1].i + G,
					dynamicarray[i][j-1].s + G + H,
					dynamicarray[i][j-1].d + G + H);
			if (LOCAL){
				if (optimal(highscore[0], highscore[1]) <
						optimal(i,j))
					highscore[0] = i; highscore[1] = j;
			}
		}
	}
	if (LOCAL)
		printf("Optimal Score: %d\n", optimal(highscore[0], highscore[1]));

	else
		printf("Optimal Score: %d\n", optimal(i-1,j-1));
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
	//printf("Path: %s\n", path);
	//printf("FILESIZE: %d\n", size);
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


int settings(const char *argv[]){
	char sline[32];
	char temp;
	char strpath[64];
	char configpath[64];

	if(argv[1] && argv[2] && (*argv[2] == '0' || *argv[2] == '1')){
		strncpy(strpath, argv[1], 64);
		strncpy(&temp, argv[2], 1);
		LOCAL = temp - '0';
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
	if (!LOCAL) {
		printf("GLOBAL\n");
	} else if (LOCAL) {
		printf("LOCAL\n");
	} else
		printf("UNKNOWN\n");
	//printf("Config: %s\n", configpath);
	printf("\n");

	FILE *fp = fopen(configpath, "r");

	if (fp) {
		while(fgets(sline, 32, fp)){
			char *tok = strtok(sline, " \t\n");
			if(strcmp(tok, "match") == 0){
				tok = strtok(NULL, "\t\n");
				//printf("%s",tok);
				MA = strtoint(tok);
				printf("Match = %d, ", MA);
			}
			else if(strcmp(tok, "mismatch") == 0){
				tok = strtok(NULL, " \t\n");
				MI = strtoint(tok);
				printf("Mismatch = %d, ", MI);
				//printf("%s\n", strtok(NULL, " \t\n"));

			}
			else if(strcmp(tok, "h") == 0){
				tok = strtok(NULL, " \t\n");
				H = strtoint(tok);
				printf("h = %d, ", H);
			}
			else if(strcmp(tok, "g") == 0){
				tok = strtok(NULL, " \t\n");
				G = strtoint(tok);
				printf("g = %d\n\n", G);
			}

		}
	}

	fclose(fp);
	printf("S1 length: %d characters\nS2 length: %d characters\n\n",
			(int)strlen(s1), (int)strlen(s2));
	return 0;

ERROR:
	//	if(fp)
	//		fclose(fp);
	printf("TEST\n");
	inputerror();
	return -1;
}

int retrace(){
	int i = strlen(s1), j = strlen(s2);
	int count = 0, o = i + j, penalty;
	int matches = 0, mismatches = 0, gaps = 0, openings = 0, max = 0;
	char res1[o], res2[o], rema[o], dir;

	max = optimal(i,j);
	if(max == dynamicarray[i][j].s)
		dir = 's';
	else if(max == dynamicarray[i][j].d)
		dir = 'd';
	else
		dir = 'i';

	while(i > 0 || j > 0){
		switch(dir){
			case 'd':
				// got score from deletion
				if (dynamicarray[i][j].d == (dynamicarray[i-1][j].d+G)){
					dir = 'd';
				}
				// got score from substitution
				else if (dynamicarray[i][j].d == (dynamicarray[i-1][j].s+H+G)){
					dir = 's';
					openings++;
				}
				// got score from insertion
				else {
					dir = 'i';
					openings++;
				}
				i--;
				gaps++;
				break;
			case 's':
				if (s1[i-1] == s2[j-1]){
					penalty = MA;
					matches++;
				} else{
					penalty = MI;
					mismatches++;
				}
				// got score from deletion
				if (dynamicarray[i][j].s ==
					(dynamicarray[i-1][j-1].d + penalty)){
					dir = 'd';
					openings++;
				}
				// got score from substitution
				else if (dynamicarray[i][j].s ==
						(dynamicarray[i-1][j-1].s + penalty)){
					dir = 's';
				}
				// got score from insertion
				else {
					dir = 'i';
					openings++;
				}
				i--;
				j--;
				break;
			case 'i':
				// got score from deletion
				if (dynamicarray[i][j].i ==
					(dynamicarray[i][j-1].d + G + H)){
					dir = 'd';
					openings++;
				}
				// got score from substitution
				else if (dynamicarray[i][j].i ==
						(dynamicarray[i][j-1].s + G + H) ){
					dir = 's';
					openings++;
				}
				// got score from insertion
				else {
					dir = 'i';
				}
				j--;
				gaps++;
				break;
			default:
				return -1;
		}
		count ++;
	}
	
	res1[count] = 0;
	rema[count] = 0;
	res2[count] = 0;
	revstring(res1);
	revstring(rema);
	revstring(res2);
//	wordwrap(res1, rema, res2);
	
	printf("Matches:%d Mismatches:%d Gaps:%d Openings:%d\n",
			matches, mismatches, gaps, openings);
	return 0;
}
/*
   int localretrace(int i, int j){ // input the position of where to start
   int m = strlen(s1), n = strlen(s2), o = m + n;
   char revs1[o], revs2[o], match[o];
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
 */

void printarray(){
	int i,j;
	printf("\t");
	for (i = 0; i < (int)strlen(s2); i++){
		printf("%c\t", s2[i]);
	}
	printf("\n  ");
	for (i = 0; i < (int)strlen(s1)+1; i++) {
		for (j = 0; j < (int)strlen(s2)+1; j++) {
			printf("%d\t", optimal(i,j));
		}
		printf("\n");
		printf("%c", s1[i]);
	}
	return;
}

#endif
