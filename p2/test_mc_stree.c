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

#include <sys/time.h>


/* ---------- PROTOTYPES -------- */	// testing driver prototypes go here...

int test_construction(void);
int test_display(void);
int test_dfs(void);
int test_bwt(void);

double double_time(struct timeval *atime);
double diff_time(struct timeval *tstart, struct timeval *tstop);


/* ---------- DEFINITIONS ------- */




/* ---------- FUNCTION DEFS ----- */	// testing driver function definitions go here...

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
int main (int argc, char* argv[])
{
	struct timeval tstart, tstop;
    
	// call some data validation first
    
	gettimeofday(&tstart, NULL);
	// test_construction()
	gettimeofday(&tstop, NULL);
	// diff_time(&start, &stop) // in ms
    
	gettimeofday(&tstart, NULL);
	// test_display()
	gettimeofday(&tstop, NULL);
	// diff_time(&start, &stop) // in ms

	gettimeofday(&tstart, NULL);
	test_dfs();
	gettimeofday(&tstop, NULL);
	// diff_time(&start, &stop) // in ms

	gettimeofday(&tstart, NULL);
	// test_bwt()
	gettimeofday(&tstop, NULL);
	// diff_time(&start, &stop) // in ms

	return (0);
}
