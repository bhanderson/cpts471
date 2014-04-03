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
#include <vector>
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

class Edge {
	int fist_char_index;
	int last_char_index;
	Node *end_node;
	Node *start_node;
	public:
		Edge();
		Edge( int first, int last, Node *parent );
		int SplitEdge();
};

class Tree{
	public:
		Node *root;
		~Tree( );
		Tree( );
		Tree( string sequence, string alphabet );
		string print_children();
		string DFS( Node *u );
		string BWT_index( string s );
	private:
		string sequence;
		string alphabet;
};

// destructor
Tree::~Tree( ){

}

Tree::Tree( ){
	root = new Node;
}

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
