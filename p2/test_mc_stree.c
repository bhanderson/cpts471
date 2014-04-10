/*
 * Author:      Bryce Handerson (bhanderson)
 *              Andrew M Bates (abates09 | andrewbates09)
 * File:        test_mc_stree.cpp
 * Project:     CptS 471 - Project 2
 * Due:         April 04, 2014
 *
 * Notes:
 * This is the testing driver for Project 2
 */

/* ---------- INCLUDES ---------- */

#include "mc_stree.c"
#include "mc_stree.h"

#include <errno.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>

/* ---------- PROTOTYPES -------- */	// testing driver prototypes go here...

long long getPhysMem();
long long getVertMem();

int validateArgs( const int argc, const char **argv );

int test_construction(void);
int test_display(void);
int test_findPath(void);
int test_nodeHop(void);
int test_ananthHop(void);
int test_IA(void);
int test_IB(void);
int test_IIA(void);
int test_IIB(void);
int test_dfs(void);
int test_bwt(void);

double double_time(struct timeval *atime);
double diff_time(struct timeval *tstart, struct timeval *tstop);


/* ---------- DEFINITIONS ------- */

struct sysinfo memInfo;


/* ---------- FUNCTION DEFS ----- */	// testing driver function definitions go here...

// get total physical memory used
long long getPhysMem() {
	return (0);
}

// get total virtual memory used
long long getVirtMem() {
	return (0);
}

// validate user input to continue, etc..
// return 0 for success, -1 for failure
int validateArgs(const int argc, const char **argv) {
	if (argc < 3) {
		printf("\nERROR: Please check your input.\n");
		printf("Format: <test executable> <input fasta> <input alphabet>\n\n");
		return -1;
	}

	printf("\nPlease verify the following information:\n\n");
	printf("Input File:\t%s\n", argv[1]);
	printf("Input Alph:\t%s\n", argv[2]);

	printf("\nIf any of this is wrong, press Ctrl+C to panic.\nOtherwise press ENTER to continue...");
	getchar();
	return (0);
}

//
int setUp(const char ** argv) {

	char inputname[64];
	char alphaname[64];

	struct stat input_st;
	struct stat alpha_st;

	stat(argv[1], &input_st);
	stat(argv[2], &alpha_st);

	unsigned int input_size = input_st.st_size;
	unsigned int alpha_size = alpha_st.st_size;

	strcpy(inputname, argv[1]);
	strcpy(alphaname, argv[2]);

	FILE *inputfile;
	FILE *alphafile;

	if ((inputfile = fopen(inputname, "r")) == NULL) {
		if (errno == ENOENT) {
			printf("\nERROR: No sequence file found.\n");
			return -1;
		}
		else {
			printf("\nERROR: Unhandled sequence read error.\n");
			return -1;
		}
	}

	if ((alphafile = fopen(alphaname, "r")) == NULL) {
		if (errno == ENOENT) {
			printf("\nERROR: No sequence file found.\n");
			return -1;
		}
		else {
			printf("\nERROR: Unhandled sequence read error.\n");
			return -1;
		}
	}

	ibuff = (char*)malloc(input_size);
	if (ibuff == NULL) {
		printf("\nERROR: Cannot allocate memory for sequence.\n");
		return -1;
	}

	abuff = (char*)malloc(alpha_size);
	if (abuff == NULL) {
		printf("\nERROR: Cannot allocate memory for alphabet.\n");
		return -1;
	}

	unsigned int i = 0;

	int ibytes = input_size;
	int abytes = alpha_size;
	char inchar = '\0';

	// read in name of sequence
	 do {
		inchar = fgetc(inputfile);
		if (inchar == '>') {
			inchar = fgetc(inputfile);
			while (inchar != '\n'  && inchar == '|') {
				if (i < 15) {
					iname[i] = inchar;
					++i;
				}
				inchar = fgetc(inputfile);
			}
			iname[i] = '\0';
		}
	} while (inchar != '\n' && inchar != EOF);

	// read in the sequence and ++inputLen
	do {
		inchar = fgetc(inputfile);
		while (inchar != EOF) {
			if (inchar != ' ' && inchar != '\n') {
				ibuff[inputLen] = inchar;
				++inputLen;
				//++i;
				//ibytes--;
			}
			inchar = fgetc(inputfile);
		}
	} while (inchar != EOF); /*ibytes > 0 && */
	ibuff[inputLen] = '\0';

	// read in alphabet
	do {
		inchar = fgetc(alphafile);
		if (inchar != ' ' && inchar != '\n') {
			abuff[alphabetLen] = inchar;
			++alphabetLen;
		}
	} while (abytes > 0 && inchar != EOF);
	abuff[alphabetLen] = '\0';

	fclose(inputfile);
	fclose(alphafile);

	return (0);
}


// run construction
int test_construction(void)
{
	// construct()
	return (0);
}

// run display
int test_display(void)
{
	// display()
	return (0);
}

int test_findPath(void){
	Node *root = makeNode( 0, NULL, NULL, 0 );
	ibuff = "banana$";
	inputLen = strlen(ibuff);
	findPath(root, &ibuff[0]);
	findPath(root, &ibuff[1]);
	findPath(root, &ibuff[2]);
	findPath(root, &ibuff[3]);
	findPath(root, &ibuff[4]);
	findPath(root, &ibuff[5]);
	findPath(root, &ibuff[6]);
	//	printChildren(root);
	dfs(root);

	return (0);
}
int test_nodeHop(void){
	Node *root = makeNode( 0, NULL, NULL, 0 );
	ibuff = "hhhhhh$";
	inputLen = strlen(ibuff);
	findPath(root, ibuff);
	findPath(root, &ibuff[1]);
	findPath(root, &ibuff[2]);
	findPath(root, &ibuff[3]);
	findPath(root, &ibuff[4]);
	findPath(root, &ibuff[5]);
	findPath(root, &ibuff[6]);
	nodeHop(root, "hhh$");

	return 0;
}

int test_ananthHop(void){
	Node *root = makeNode( 0, NULL, NULL, 0 );
	ibuff = "hhhhhh$";
	int ijk = 3;
	inputLen = strlen(ibuff);
	findPath(root, ibuff);
	findPath(root, &ibuff[1]);
	findPath(root, &ibuff[2]);
	findPath(root, &ibuff[3]);
	findPath(root, &ibuff[4]);
	findPath(root, &ibuff[5]);
	findPath(root, &ibuff[6]);
	ananthHop(root, root->children[0], "hhh$", &ijk);

	return 0;
}
int test_IA(void){
	Node *root = makeNode( 0, NULL, NULL, 0 );
	Node *a = makeNode( 8, root, "a", 1);
	addChild(root, a);
	a->suffixLink = root;
	Node *na = makeNode( 11, root, "na", 2);
	na->suffixLink = a;
	addChild(root, na);
	Node *ana = makeNode( 9, a, "na", 3);
	addChild(a, ana);
	Node *dollar = makeNode( 10, a, "$", 2);
	addChild(a, dollar);

	//insert( "nana", root, ana);

	return 0;
}

// run enumerate
int test_dfs(void)
{
	// enumerate()
	return (0);
}

// run bwt
int test_bwt(void)
{
	// bwt()
	return (0);
}


// convert time to some double ms
double double_time(struct timeval *atime)
{
	return ((atime->tv_sec + (atime->tv_usec/1000000.0)) * 1000.0);
}

// get difference of time
double diff_time(struct timeval *tstart, struct timeval *tstop)
{
	return (double_time(tstop) - double_time(tstart));
}

// where it all starts
int main (int argc, const char *argv[])
{
	struct timeval tstart, tstop;

	long long startPMem, constructPMem, displayPMem, dfsPMem, bwtPMem = 0;
	long long startVMem, constructVMem, displayVMem, dfsVMem, bwtVMem = 0;

	// set initial memory usage - to do comparisons of everything else
	startPMem = getPhysMem();
	startVMem = getVirtMem();

	// call some data validation first
	if (validateArgs(argc, argv) < 0) {
		return -1;
	}

	// read in stuf if all validation went correctly
	if (setUp(argv) < 0) {
		printf("\nERROR: something went wrong.\n");
		return -1;
	}

	gettimeofday(&tstart, NULL);
	//test_findPath();
	//test_IA();
	//test_nodeHop();
	gettimeofday(&tstop, NULL);
	// diff_time(&start, &stop) // in ms

	gettimeofday(&tstart, NULL);
	// test_construction()
	gettimeofday(&tstop, NULL);
	printf("ST Constructon: %f ms\n", diff_time(&tstart, &tstop));

	gettimeofday(&tstart, NULL);
	// test_display()
	gettimeofday(&tstop, NULL);
	printf("ST Display: %f ms\n", diff_time(&tstart, &tstop));

	gettimeofday(&tstart, NULL);
	// test_dfs();
	gettimeofday(&tstop, NULL);
	printf("ST DFS: %f ms\n", diff_time(&tstart, &tstop));

	gettimeofday(&tstart, NULL);
	// test_bwt()
	gettimeofday(&tstop, NULL);
	printf("ST BWT: %f ms\n", diff_time(&tstart, &tstop));

	return (0);
}
