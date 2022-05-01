#ifndef SERIAL_H
#define SERIAL_H

#include <vector>
#include <queue>
#include <iostream>
#include "../graphs.h"

void bfs(std::vector<std::vector<long long> > &G, std::vector<bool> &visited, long long origin);
void bfs(const adjacencyList &G, bfs_node origin,void (*action)(bfs_node));

#endif

