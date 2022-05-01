#include "serial_bfs.h"
#include <math.h>

using namespace std;

void bfs(vector<vector<long long> >& G, vector<bool> &visited, long long origin){
    long long n = G.size();
    queue<long long> q;
    visited[origin] = true;

    q.push(origin);
    while(!q.empty()){
        long long node = q.front();
        q.pop();
        for(long long &neighbor : G[node]){
            if(!visited[neighbor]){
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

}

void bfs(const adjacencyList &G, bfs_node origin,void (*action)(bfs_node)){
    std::queue<bfs_node> q;
    std::vector<bool>seen(G.size(),false);
    
    q.push(origin);
    seen[origin]=true;

    while(!q.empty()){
        bfs_node node = q.front();
        q.pop();
        if(action!=NULL)
            action(node);
        for(auto i = G.at(node).begin();i!=G.at(node).end();i++){
            bfs_node neighbor = *i;
            if(seen[neighbor]==false){
                q.push(neighbor);
                seen[neighbor]=true;
            }
        }
    }
}
