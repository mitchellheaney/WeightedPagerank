// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix

///////////////////////////////// NOTE ///////////////////////////////////
//
// The implementation of this Graph ADT was copied from week 8 of 
// UNSW's COMP2521 21T3 lab question, with minor modifications.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent, 1 if adjacent
};

static int  validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

bool GraphInsertEdge(Graph g, Edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight > 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

int get_inlink_num(Graph g, int index) {

    int inlink = 0;
    for (int i = 0; i < g->nV; i++) {
        for (int j = 0; j < g->nV; j++) {
            if (g->edges[i][j] != 0.0 && j == index) {
                inlink++;
            }
        }
    }
    return inlink;
}

int get_outlink_num(Graph g, int index) {

    int outlink = 0;
    for (int i = 0; i < g->nV; i++) {
        for (int j = 0; j < g->nV; j++) {
            if (g->edges[i][j] != 0.0 && i == index) {
                outlink++;
            }
        }
    }
    return outlink;
}

void return_reference_inlinks(int array[], int index, Graph g) {

    int count = 0;
    for (int i = 0; i < g->nV; i++) {
        for (int j = 0; j < g->nV; j++) {
            if (g->edges[i][j] != 0.0 && j == index) {
                array[count] = i;
                count++;
            }
        }
    }
}

int get_win_denominator(int inlink_ref, int index, Graph g) {

    int result = 0;
    for (int i = 0; i < g->nV; i++) {
        for (int j = 0; j < g->nV; j++) {
            // If we are in the correct row
            if (g->edges[i][j] != 0.0 && i == inlink_ref) {
                int indegree_col = get_inlink_num(g, j);
                result += indegree_col;
            }
        }
    }
    return result;
}

double get_wout_denominator(int inlink_ref, int index, Graph g) {

    double result = 0;
    for (int i = 0; i < g->nV; i++) {
        for (int j = 0; j < g->nV; j++) {
            // If we are in the correct row
            if (g->edges[i][j] != 0.0 && i == inlink_ref) {
                double outdegree_col = get_outlink_num(g, j);
                // If the outdegree is 0, make it 0.5
                if (outdegree_col == 0) {
                    outdegree_col = 0.5000000;
                }
                result += outdegree_col;
            }
        }
    }
    return result;
}

static int validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

