#ifndef GRAPH_GEN_H
#define GRAPH_GEN_H 
#include <vector>
#include <iostream>
#include "../graphs.h"
adjacencyList generate_graph_list(int nodes,int min_neighbors,int max_neighbors,int seed);
adjacencyList generate_graph_list_from_text();
adjacencyList generate_graph_list_from_textfile(std::string textfile);
adjancencyMatrix generate_graph_matrix(int nodes,int min_neighbors,int max_neighbors,int seed);
void print_graph(adjacencyList g);
void print_graph(adjancencyMatrix g);

#endif
