#ifndef OMP_BFS
#define OMP_BFS

#include <omp.h>
#include <vector>
#include <queue>
#include "../graphs.h"
//bfs with input G as adjacency_list
void bfs_omp(const adjacencyList &G, bfs_node origin,void (*act)(bfs_node),int NUM_THREADS);

#endif