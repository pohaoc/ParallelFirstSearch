#include "serial_bfs.h"
#include <chrono>

using namespace std::chrono;

int main(){
    std::ios_base::sync_with_stdio(false); //fast printing
    long long n,m;

    // # of verticies is a 2^n
    // # of edges is |V| * m
    std::cin >> n >> m; // expects kroncker's graph 


    std::vector<std::vector<long long>> G(n, std::vector<long long>(0));

    for(long long i =0 ; i < m ; i++){
        long long a,b;
        std::cin >> a >> b;
        G[a].push_back(b);
    }

    std::vector<bool> visited(n);

    //sample random vertex
    long long random_source = rand() % n;

    std::cout << "=======  Serial BFS Begins  =======\n";
    auto start = high_resolution_clock::now();

    //for(int i = 0 ; i < n ; i++){
        //if(!visited[i]){
            bfs(G,visited, random_source);
        //}
    //}

    auto end = high_resolution_clock::now();
    std::cout << "======= Serial BFS Finishes ======= \n";
    auto measurement = duration_cast<nanoseconds>(end - start);
    std::cout << measurement.count() << " nanoseconds\n";
}

