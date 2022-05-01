#include "omp_bfs.h"
#include <iostream>
#include <list>
void bfs_omp(const adjacencyList &G, bfs_node origin,void (*action)(bfs_node),int NUM_THREADS){
    omp_set_num_threads(NUM_THREADS);
    std::list<bfs_node> q;
    std::vector<bool>seen(G.size(),false);
    
    q.push_back(origin);
    seen[origin]=true;

    while(!q.empty()){
        std::list<bfs_node> newq;
        for(bfs_node node = q.front();!q.empty();q.pop_front()){
            if(action!=NULL)
                action(node);
            for(auto i = G.at(node).begin();i!=G.at(node).end();i++){
                bfs_node neighbor = *i;
                if(seen[neighbor]==false){
                    newq.push_back(neighbor);
                    seen[neighbor]=true;
                }
            }
        }
        q=newq;
    }
}