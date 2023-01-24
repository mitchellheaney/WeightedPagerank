# WeightedPagerank

## About the Project

The Weighted Pagerank project was a task assigned to me in university during the course COMP2521 in
2021 as a first year learning data structures for the first time. It is emulating Google's first ever 
algorthim used to rank popularity of web pages within its search engine, through counting the number
of quality links to a page; yeilding a measurement of website importance. Today's page ranking system
is now inspired by monetary influence. Implementation of the algorithm was inspired from 
https://en.wikipedia.org/wiki/PageRank.

It utilises a directed graph ADT to increment the number of inlinks a web pages using mock pages found 
in the 'ex1' folder. Each mock page contains other mock sites it has inlinks to as well as mock 
descriptions of the site.

## How to Run

1. The user must download the ZIP file into their directory.
2. Type in 'make' assuming relevant 'Makefile' extensions are downloaded.
> make
3. Change to 'ex1' directory
> cd ex1
4. Enter
> ./pagerank (damping factor) (sum of pagerank differences) (num. iterations)

*NOTE*

**Damping Factor**: The probability a 'surfing' web user will continue to click on links; usually 0.85
**Pagerank Differences**: Popularity differences 
**Number of Iterations**: Amount of times the algorithm executes the mathematical model.

5. Sorted urls in ex1 folder will appear in descending order in 'pagerankList.txt'

## Drawbacks/Learning Aspirations

During this university assignment at UNSW, it taught me the drawbacks of using an adjacency matrix
for graph implementations, mainly wasted space as if links made were sparse, memory optimisation
is diminished as a continuous worst case O(n^2) for traversals. Another dislike I had was the 
nature of the mock pages. It would have been more rewarding to import real websites from Google API
and see real popularity of sites, but this is a learning opportunity I wish to study further. Along
with DFS and BFS algorithm's, the project has educated me upon key data algorithms that support 
procedural programming.



