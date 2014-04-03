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
#ifndef _MC_STREE_H_
#define _MC_STREE_H_
#include <cctype>
#include <cstdio>
#include <string>
using namespace std;


/* ---------- DEFINITIONS ------- */

class Node{
	unsigned int id;
	Node *link;
	Node *parent;
	string parent_edge_label;
	vector<Node*> children;
	int depth;

};

class Tree{
	public:
		Node * root;
		~Tree( );
		Tree( string sequence, string alphabet );
		string print_children();
		string DFS( Node *u );
		string BWT_index( string s );
	private:
		string sequence;
		string alphabet;
};

// destructor
Tree::~Tree( );

// constructor
Tree::Tree( string sequence, string alphabet ){

}


string print_children(){

}

string DFS( Node *u ){

}

string BWT_index( string s ){

}
/* ---------- PROTOTYPES -------- */

#endif
