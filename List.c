// Implementation of the String List ADT

///////////////////////////////// NOTE ///////////////////////////////////
//
// The implementation of this List ADT was copied from week 5 of 
// UNSW's COMP2521 21T3 lab question, with minor modifications.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

typedef struct node *Node;
struct node {
    char  *s;
    Node   next;
    int reference;  
};

struct list {
    Node   head;
    Node   tail;
    int    size;
};

static Node newNode(char *s, List l);
static char *myStrdup(char *s);

////////////////////////////////////////////////////////////////////////

List ListNew(void) {
    List l = malloc(sizeof(*l));
    if (l == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

void ListFree(List l) {
    Node curr = l->head;
    while (curr != NULL) {
        Node temp = curr;
        curr = curr->next;
        free(temp->s);
        free(temp);
    }
    free(l);
}

void ListAppend(List l, char *s) {
    Node n = newNode(s, l);
    if (l->head == NULL) {
        l->head = n;
    } else {
        l->tail->next = n;
    }
    l->tail = n;
    l->size++;
}

static Node newNode(char *s, List l) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    n->s = myStrdup(s);
    n->next = NULL;
    n->reference = ListSize(l);
    return n;
}

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

int  ListSize(List l) {
    return l->size;
}

char *return_list_name(int index, List l) {

    Node current = l->head;
    if (current == NULL) {
        return 0;
    }
 
    int i = 0;
    while (current != NULL) {
        if (i == index) {
            break;
        } else {
            current = current->next;
            i++;
        }
    }
    // Check if the index exceeded
    if (i >= ListSize(l)) {
        printf("Exceeded the list length limit.\n");
        return 0;
    }
    return current->s;
}

void return_reference_for_list(int array[], List sub, List all) {
    if (!ListSize(sub)) {
        return;
    }
    int count = 0;

    Node current_in_sub = sub->head;
    while (current_in_sub != NULL) {

        Node current_in_all = all->head;
        while (current_in_all != NULL) {
            // If we found a matching name in both urls, return the index from 
            // the list 'all'
            if (strcmp(current_in_all->s, current_in_sub->s) == 0) {                
                array[count] = current_in_all->reference;
                count++;
                break;
            }
            current_in_all = current_in_all->next;
        }
        current_in_sub = current_in_sub->next;
    }
}

int return_node_reference(char *name, List l) {

    Node current = l->head;
    while (current != NULL) {
        if (strcmp(name, current->s) == 0) {
            break;
        }
        current = current->next;
    }
    return current->reference;
}