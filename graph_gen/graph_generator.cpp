#include "graph_generator.h"
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
//directed
adjacencyList generate_graph_list(unsigned long nodes,unsigned long min_neighbors,unsigned long max_neighbors,int seed){
    unsigned long neighbor_range=max_neighbors-min_neighbors;
    assert(neighbor_range>0);
    srand(seed);
    adjacencyList graph(nodes,alist());
    for(unsigned long i=0;i<nodes;i++)
    {
        unsigned long nneighbors = rand()%neighbor_range+min_neighbors;
        std::vector<bool> neighbors(nodes,false);
        for(unsigned long j=0;j<nneighbors;j++){
            bfs_node neighbor=rand()%nodes;
            while(neighbor==i || neighbors[neighbor])
                neighbor=rand()%nodes;
            neighbors[neighbor]=true;
            graph[i].push_back(neighbor);
        }
    }
    return graph;
}

std::vector<std::vector<bool> > generate_graph_matrix(unsigned long nodes,unsigned long min_neighbors,unsigned long max_neighbors,int seed){
    bfs_node neighbor_range=max_neighbors-min_neighbors;
    assert(neighbor_range>0);
    srand(seed);
    std::vector<std::vector<bool> > graph(nodes,std::vector<bool>(nodes,false));
    for(unsigned long i=0;i<nodes;i++)
    {
        unsigned long long nneighbors = rand()%neighbor_range+min_neighbors;
        for(unsigned long j=0;j<nneighbors;j++){
            bfs_node neighbor=rand()%nodes;
            while(neighbor==i || graph[i][neighbor])
                neighbor=rand()%nodes;
            graph[i][neighbor]=true;
        }
    }
    return graph;
}

void print_graph(adjacencyList g){
    std::stringstream ss;
    unsigned long long m=0;
    unsigned long long n = g.size();
    // std::cout<<"n="<<n<<std::endl;
    for(unsigned long i=0;i<n;i++){
        ss<<i<<": ";
        for(auto j=g[i].begin();j!=g[i].end();j++){
            ss<<*j<<",";
            m++;
        }
        ss<<std::endl;
    }
    std::cout<<"n m\n"<<n<<" "<<m<<std::endl;
    std::cout<<ss.str();
}
void print_graph(std::vector<std::vector<bool>> g){
    std::stringstream ss;
    unsigned long m=0;
    unsigned long n = g.size();
    for(unsigned long i=0;i<n;i++){
        for(unsigned long j=0;j<n;j++){
            ss<<g[i][j]<<" ";
            m+=g[i][j];
        }
        ss<<std::endl;
    }
    std::cout<<"n m\n"<<n<<" "<<m<<std::endl;
    std::cout<<ss.str();
}
adjacencyList generate_graph_list_from_text(){
    
    bfs_node n,m;

    // # of verticies is a 2^n
    // # of edges is |V| * m
    std::cin >> n >> m; // expects kroncker's graph 

    adjacencyList graph(n,alist(0));

    for(bfs_node i =0 ; i < m ; i++){
        bfs_node a,b;
        std::cin >> a >> b;
        graph[a].push_back(b);
    }
    return graph;
}
adjacencyList generate_graph_list_from_textfile(std::string textfile){
    std::ifstream filestream;
    filestream.open(textfile);

    bfs_node n,m;

    // # of verticies is a 2^n
    // # of edges is |V| * m
    filestream >> n >> m; // expects kroncker's graph 

    adjacencyList graph(n,alist(0));

    for(bfs_node i =0 ; i < m ; i++){
        bfs_node a,b;
        filestream >> a >> b;
        graph[a].push_back(b);
    }
    return graph;
}
