
#include "../pthread/pthread_bfs.h"
#include "../OpenMP/omp_bfs.h"
#include "../graph_gen/graph_generator.h"
#include <math.h>
#include <chrono>
#include "../serial/serial_bfs.h"
using namespace std::chrono;
#define NUM_THREADS 32
#ifndef NODES
#define NODES 100000
#endif
#define NTRIALS 5

int main(){
       
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto sumtime=duration.count();
    
    std::cout<<"N,serial_time,pthread_time,omp_time\n";
    int error = 0;
    for (int nodes=20;nodes<=NODES;nodes*=2)
    {
        int min = nodes/10;
        int max = min*2;
        adjacencyList g = generate_graph_list(nodes,nodes/20,nodes/5,100);
        

        std::cout<<nodes<<",";
        
        sumtime=0;
        for(int i = 0;i<NTRIALS;i++){
            start = high_resolution_clock::now();
            // bfs(g,0,action1);
            bfs(g,0,NULL);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            sumtime+=duration.count();
        }
        std::cout<<sumtime/NTRIALS<<",";
        sumtime=0;
        for(int i = 0;i<NTRIALS;i++){
            start = high_resolution_clock::now();
            bfs_pt(g,0,NULL,NUM_THREADS);
            // bfs_pt(g,i,action2,NUM_THREADS);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            sumtime+=duration.count();
        }
        std::cout<<sumtime/NTRIALS<<",";
        sumtime=0;
        for(int i = 0;i<NTRIALS;i++){
            start = high_resolution_clock::now();
            bfs_omp(g,0,NULL,NUM_THREADS);
            // bfs_pt(g,i,action2,NUM_THREADS);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            sumtime+=duration.count();
        }
        std::cout<<sumtime/NTRIALS<<std::endl;
    }
    return 0;
}