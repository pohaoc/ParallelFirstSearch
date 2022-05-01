#ifndef PTHREAD_BFS
#define PTHREAD_BFS

#include <pthread.h>
#include <vector>
#include <queue>
#include "../graphs.h"
//bfs with input G as adjacency_list
void bfs_pt(const adjacencyList &G, bfs_node origin,void (*act)(bfs_node),int NUM_THREADS);
// void bfs(const adjacencyList &G, bfs_node origin,void (*action)(bfs_node));

#endif