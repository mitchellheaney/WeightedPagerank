// Interface to the Undirected Weighted Graph ADT
// - Vertices are identified by integers between 0 and nV - 1,
//   where nV is the number of vertices in the graph
// - Weights are doubles and must be positive
// - Self-loops are not allowed

////////////////////////////////// NOTE ///////////////////////////////////
//
// The implementation of this Graph ADT was copied from week 8 of 
// UNSW's COMP2521 21T3 lab question, with minor modifications.

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct graph *Graph;

typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
    Vertex v;
    Vertex w;
    double weight;
} Edge;


/**
  * Creates a new instance of a graph
 */
Graph  GraphNew(int nV);

/**
 * Frees all memory associated with the given graph
 */
void   GraphFree(Graph g);

/**
 * Inserts  an  edge into a graph. Does nothing if there is already an
 * edge between `e.v` and `e.w`. Returns true if successful, and false
 * if there was already an edge.
  */
bool   GraphInsertEdge(Graph g, Edge e);

/**
 * Gets the indegree of an url, provided the url index is passed in
 * correctly. Returns the indegree int.
 */
int get_inlink_num(Graph g, int index);

/**
 * Gets the outdegree of an url, provided the url index is passed in
 * correctly. Returns the outdegree int.
 */
int get_outlink_num(Graph g, int index);

/**
 * Populates the array with the reference numbers of the urls that are
 * incoming links to the url passed in at index. Analyses the graph 'g'
 * for these incoming links.
 */
void return_reference_inlinks(int array[], int index, Graph g);

/**
 * Returns the Win denominator when given the 2 indexes for the
 * Win to be calcualted in between. This is used to correctly find
 * the sum of reference pages that are indegrees to the index url.
 */
int get_win_denominator(int inlink_ref, int index, Graph g);

/**
 * Returns the Wout denominator when given the 2 indexes for the
 * Wout to be calcualted in between. This is used to correctly find
 * the sum of reference pages that are outdegrees to the index url.
 */
double get_wout_denominator(int inlink_ref, int index, Graph g);

#endif
