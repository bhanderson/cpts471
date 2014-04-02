/*
 * Author:      Bryce Handerson (bhanderson)
 *              Andrew M Bates (abates09 | andrewbates09)
 * File:        mc_stree.h
 * Project:     CptS 471 - Project 2
 * Due:         April 04, 2014
 * 
 * Notes:
 * The goal of this project is to implement the McCreight's suffix tree con-
 *      struction algorithm, given an input string s (in the FASTA file format)
 *      and a file containing the input alphabet.
 */


/* ---------- INCLUDES ---------- */

#include <cctype>
#include <cstdio>
#include <string>


/* ---------- DEFINITIONS ------- */

class Tree{
	public:
		Tree(string sequence, string alphabet);
	private:
		string sequence;
		string alphabet;
};

// destructor
Tree::~Tree( );

// constructor
Tree::Tree( string sequence, string alphabet ){

}

/* ---------- PROTOTYPES -------- */


