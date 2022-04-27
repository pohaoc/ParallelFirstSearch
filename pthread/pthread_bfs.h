#ifndef PTHREAD_BFS
#define PTHREAD_BFS

#include <pthread.h>
#include <vector>
#include <queue>
#include "../graphs.h"
//bfs with input G as adjacency_list
void bfs_pt(const adjacencyList &G, int origin,void (*action)(int),int NUM_THREADS);
void bfs(const adjacencyList &G, int origin,void (*action)(int));

#endif