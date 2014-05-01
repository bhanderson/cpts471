#ifndef _DPTABLE_H_
#define _DPTABLE_H_

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
} DP_cell;

DP_cell **dynamicarray;

void print_menu();

int dynamicfree();

void wordwrap(char *a1, char *m, char *a2);

char *revstring(char *str);

void inputerror();

int threemax(int a, int b, int c);

int allarr(int m, int n);

int optimal(int i, int j);

int localAlign(char *s1, char *s2, unsigned int length,
				unsigned int *matches,
				unsigned int *mismatches,
				unsigned int *gaps,
				unsigned int *openings);

int align(char *s1, char *s2);

int dynamicstrinput(char *path);

int strtoint(char *str);

int settings(const char *argv[]);

int mapRetrace(unsigned int *matches, unsigned int *mismatches, unsigned int *gaps, unsigned int *openings);

int retrace();

void printarray();

#endif // _DPTABLE_H_
