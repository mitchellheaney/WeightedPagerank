// Interface to the String List ADT

///////////////////////////////// NOTE ///////////////////////////////////
//
// The implementation of this List ADT was copied from week 5 of 
// UNSW's COMP2521 21T3 lab question, with minor modifications.

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct list *List;

// Creates a new empty list
// Complexity: O(1)
List ListNew(void);

// Frees all memory allocated for the given list
// Complexity: O(n)
void ListFree(List l);

// Adds a string to the end of the list. Makes a copy of the string
// before adding it to the list.
// Complexity: O(1)
void ListAppend(List l, char *s);

// Returns the number of items in the list
// Complexity: O(1)
int  ListSize(List l);

// Given an index in a list and a list, this function returns
// the name of the url in that index in that list
char *return_list_name(int index, List l);

// Populates the array passed in with the relevant reference numbers
// so we can create edges in the graph between urls.
void return_reference_for_list(int array[], List sub, List all);

// Returns the reference number of the url name in the lsit given.
// Must assume the name is already in that list given.
int return_node_reference(char *name, List l);

#endif

