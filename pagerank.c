// COMP2521 Assignment 2

// Written by: Mitchell Heaney
// Date: 10/11/2021 - 19/11/2021


//////////////////////////// ABOUT THE PROGRAM ////////////////////////////
//
// This program accepts 3 command line arguments which include the damping
// factor, difference in page rankings and number of iterations respectively.
// These inputs given are used to calculate the 'Weighted Pagerank Formula' 
// previously used by Google search systems by ranking pages in terms of 
// popularity and the dependence of its reference page's popularity. Given
// url's in collection.txt, the program prints the results in the file 
// pagerankList.txt in the format of url name, number of outdegrees and
// its weighted page rank respectively.
//
// An adjacency matrix if used to implement the graph structure with a
// supporting list ADT specified in the directory.


#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include own files
#include "Graph.h"
#include "List.h"

// Own defines
#define MAX_FILE_LEN 100

// Own functions.
void get_individual_urls(List l, char *file_examined);
double calculate_win(int inlink_ref, int index, Graph g);
double calculate_wout(int inlink_ref, int index, Graph g);
double calculate_page_rank(int curr, int inlink_index[], int num_inlinks, 
    double prev_weights[], Graph g, double d, int n);
double sum_of_diff(int n, double prev[], double next[]);
void sort_weights(double array[], double weights[], int num);
void get_sorted_index(double ordered[], double unordered[], int num, 
    int store[]);


int main(int argc, char *argv[]) {

    // Initialise all command line arguments.
    double damping_factor = atof(argv[1]);
    double diff_pr_sum = atof(argv[2]);
    int max_iterations = atoi(argv[3]); 

    // Make a list of the urls from collection.txt
    char *file_to_open = "collection.txt";             
    FILE *collection_file = fopen(file_to_open, "r");
    if (collection_file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    // Holds the urls from collection.txt file
    List url_list = ListNew();         

    // Loop through the file and append all url names to url_list
    char url_name[MAX_FILE_LEN + 1];
    while (fscanf(collection_file, "%s", url_name) != EOF) {
        ListAppend(url_list, url_name);
    }
    fclose(collection_file);

    // Make a new graph to create in-edges and out-edges between urls
    // depending if they have inlink(s) or outlink(s)
    Graph page_graph = GraphNew(ListSize(url_list));

    // For each url in url_list, make a link between the current url's
    // page and the incoming pages inside of its url file
    for (int i = 0; i < ListSize(url_list); i++) {

        // Return the name of the url being analysed inside of each_url_name
        char each_url_name[MAX_FILE_LEN + 1];
        strcpy(each_url_name, return_list_name(i, url_list));

        // Make a new list to store the current url's incoming individiual urls
        List individual_urls = ListNew();
        get_individual_urls(individual_urls, each_url_name);

        // For each url inside of the individual_urls list, return the 
        // reference number of the url so we can make an edge in page_graph.
        int len_list = ListSize(individual_urls);
        int array[len_list];
        return_reference_for_list(array, individual_urls, url_list);   

        // If the url inside of the current url's file is not itself nor 
        // is it a parallel edge, make an edge in the graph.
        for (int j = 0; j < len_list; j++) {
            if (return_node_reference(each_url_name, url_list) != array[j]) {
                Edge page_link;
                page_link.v = return_node_reference(each_url_name, url_list);
                page_link.w = array[j];
                page_link.weight = 1.0;

                GraphInsertEdge(page_graph, page_link);
            }
        }
        // Free each malloced list
        ListFree(individual_urls); 
    }

    // Get the length of the number of urls in the collection file
    int collection_num_url = ListSize(url_list);

    // Create an array holding the initial weights for each page inside of 
    // previous_pagerank
    double previous_pagerank[collection_num_url];
    for (int i = 0; i < collection_num_url; i++) {
        previous_pagerank[i] = 1.0 / collection_num_url;
    }

    // Second array to update the weighted page algorithm
    double next_pagerank[collection_num_url];

    double diff = diff_pr_sum;
    int iteration = 0;

    // Loop to calcualte the weighted pagerank algorithm
    while ((iteration < max_iterations) && (diff >= diff_pr_sum)) {

        for (int each_url = 0; each_url < ListSize(url_list); each_url++) {

            // Gets the number of incoming urls for the url being analysed
            int inlink = get_inlink_num(page_graph, each_url);

            // Holds the references for the url's to be identified as inlinks
            int array_inlink[inlink];
            return_reference_inlinks(array_inlink, each_url, page_graph);

            // Update the next array with the output of the equation
            next_pagerank[each_url] = 
                calculate_page_rank(
                    each_url, 
                    array_inlink, 
                    inlink, previous_pagerank, 
                    page_graph, 
                    damping_factor, 
                    collection_num_url
                );
        }
        // Calculate the difference between the previous weight array and the 
        // updated array. 
        diff = sum_of_diff(collection_num_url, previous_pagerank, 
            next_pagerank);

        // Update the previous array for the next possible iteration
        for (int i = 0; i < collection_num_url; i++) {
            previous_pagerank[i] = next_pagerank[i];
        }
        iteration++;
    }    

    // Order the weights in descending order and adjust indexes accordingly
    double order_weights[collection_num_url];
    int index_order[collection_num_url];
    sort_weights(order_weights, next_pagerank, collection_num_url);
    get_sorted_index(order_weights, next_pagerank, collection_num_url, 
        index_order);

    // Write the necessary contents to the pagerankList.txt file
    FILE *output_file = fopen("pagerankList.txt", "w");
    for (int i = 0; i < collection_num_url; i++) {
        fprintf(output_file, "%s, %d, %.7lf\n",                      
            return_list_name(index_order[i], url_list), 
            get_outlink_num(page_graph, index_order[i]), 
            order_weights[i]
        );
    }
    fclose(output_file);

    
    // Free malloced data 
    ListFree(url_list);
    GraphFree(page_graph);

    return 0;
}


/////////////////////////// HELPER FUNCTIONS /////////////////////////////


// This function returns the urls that are incoming into the url being
// passed into file_examined. It will populate urls into the list 'l'.
void get_individual_urls(List l, char *file_examined) {

    // If empty list
    if (l == NULL) {
        printf("No contents in the list\n");
        exit(1);
    }

    // Make a new string to hold the string of the file_examined
    int len_file_name = strlen(file_examined) + 1;
    char temp_filename[len_file_name];
    snprintf(temp_filename, sizeof temp_filename, "%s", file_examined);

    // Append '.txt' to the temp_filename
    char text_suffix[5] = ".txt";
    strcat(temp_filename, text_suffix);
    
    FILE *open_file_examined = fopen(temp_filename, "r");

    // Scan the file and append the urls that are incoming into temp_filename
    // until we reach the end
    char url_name[MAX_FILE_LEN + 1];
    // Loop until we reach the end of the delimiting string "#end"
    while ((fscanf(open_file_examined, "%s", url_name) != EOF) && 
        (strcmp(url_name, "#end") != 0)) {

            // If we have started from the correct delimiter, keep looping
            if (strcmp(url_name, "#start") == 0 || strcmp(url_name, 
                "Section-1") == 0) {
                    continue;
            } 
            // If we find a duplicate, don't append this but keep looping
            else if (strcmp(url_name, file_examined) == 0) {
                continue;
            }
            else {
                ListAppend(l, url_name);
            }
    }
    fclose(open_file_examined);
}   

// This function returns the variable double type inside of the weighted
// page calculation. It is given the current url index, its incoming url
// data, previous weights and also other constants.
double calculate_page_rank(int curr, int inlink_index[], int num_inlinks, 
    double prev_weights[], Graph g, double d, int n) {

        // Calculate the constant
        double result = 0;
        double constant = (1.0 - d) / n;

        // Calculate the sum of the previous weights multiplied by the Win and 
        // Wout of the url with the 'curr' reference number
        double sum_inlink_weights = 0.0;
        for (int i = 0; i < num_inlinks; i++) {
            sum_inlink_weights += (prev_weights[inlink_index[i]] * 
                calculate_win(inlink_index[i], curr, g) * 
                    calculate_wout(inlink_index[i], curr, g));
        }

        // Finalise the calculation
        result = constant + (d * sum_inlink_weights);
        return result;
}

// This function calcualtes the Win double when given indexes 
// inside of inlink_ref and index via the graph 'g'
double calculate_win(int inlink_ref, int index, Graph g) {

    double get_u_indegree = get_inlink_num(g, index);
    double denominator = get_win_denominator(inlink_ref, index, g);

    return get_u_indegree / denominator;
}   

// This function calcualtes the Wout double when given indexes 
// inside of inlink_ref and index via the graph 'g'
double calculate_wout(int inlink_ref, int index, Graph g) {

    double get_u_outdegree = get_outlink_num(g, index);

    // If the Wout is 0, make it equal 0.5
    if (get_u_outdegree == 0) {
        get_u_outdegree = 0.5;
    }

    double denominator = get_wout_denominator(inlink_ref, index, g);
    return (double)get_u_outdegree / denominator;
}

// This function calcualtes the difference between the prev array of weights
// and the next array of weights, returning it as a double
double sum_of_diff(int n, double prev[], double next[]) {

    double final_sum = 0;
    for (int i = 0; i < n; i++) {
        final_sum += fabs(next[i] - prev[i]);
    }

    return final_sum;
}

// This function sorts the weights array and returns it sorted inside of 
// 'array'. Selection sorting method is used
void sort_weights(double array[], double weights[], int num) {

    double low = 0.0;
    double high = 1;

    for (int i = 0; i < num; i++) {
        low = 0.0;
        int j = 0;
        for (j = 0; j < num; j++) {
            if (weights[j] > low && weights[j] < high) {
                low = weights[j];
            }
        }
        j = 0;
        array[i] = low;
        high = low;
    }
}

// This function returns the reference numbers of the urls that have weighted
// pageranks. It analyses the ordered and unordered array to return the 
// reference numbers inside of 'store' so the outdegrees can be calcualted 
void get_sorted_index(double ordered[], double unordered[], int num, 
    int store[]) {

        for (int i = 0; i < num; i++) {
            int j = 0;
            for (j = 0; j < num; j++) {
                // Once we found the matching index, break from loop
                if (ordered[i] == unordered[j]) {
                    break;
                }
            }
            // Populate the array 'store'
            store[i] = j;
        }
}