
#include "pthread_bfs.h"
#include "../graph_gen/graph_generator.h"
#include <math.h>
#include <chrono>
using namespace std::chrono;
#define NUM_THREADS 28
#define NODES 10000

static double outputs1[NODES+1]={1};
static double outputs2[NODES+1]={1};
static double inputs[NODES];

void action1(int n){
    for(int i=0;i<=n;i++)
        outputs1[n]*=inputs[i];
}
        
void action2(int n){
    for(int i=0;i<=n;i++)
        outputs2[n]*=inputs[i];
}
int main(){
    adjacencyList g = generate_graph_list(20,5,20,100);
    // print_graph(g);

    for(int i=0;i<=NODES;i++){
        inputs[i]=i;
        outputs1[i]=1;
        outputs2[i]=1;
    }
    inputs[0]=1;
    for(int i=0;i<20;i++)
        std::cout<<inputs[i]<<" ";
    std::cout<<std::endl;

    std::cout<<"========serial bfs========\n";
    auto start = high_resolution_clock::now();
    bfs(g,0,action1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout<<"time (ms): "<<duration.count()<<std::endl;
    std::cout<<"========pthread bfs========\n";
    start = high_resolution_clock::now();
    bfs_pt(g,0,action2,NUM_THREADS);
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    std::cout<<"time (ms): "<<duration.count()<<std::endl;
    for(int i=0;i<NODES;i++)
        if(outputs1[i]!=outputs2[i]){
            std::cout<<"ERROR: "<<outputs1[i]<<" != "<<outputs2[i]<<std::endl;
            exit(1);
        }
    std::cout<<"Sucess!\n";
    for(int i=0;i<20;i++)
        std::cout<<outputs1[i]<<" ";

    std::cout<<"csv:\n\n";
    std::cout<<"N,serial_time,pthread_time\n";
    int error = 0;
    for (int nodes=NODES/10;nodes<=NODES;nodes+=NODES/10)
    {
        int min = nodes/10;
        int max = min*2;
        adjacencyList g = generate_graph_list(nodes,5,20,100);
        // print_graph(g);
        for(int i=0;i<nodes;i++)
            inputs[i]=i;

        std::cout<<nodes<<",";

        start = high_resolution_clock::now();
        bfs(g,0,action1);
        // bfs(g,0,NULL);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        std::cout<<duration.count()<<",";
        
        start = high_resolution_clock::now();
        // bfs_pt(g,0,NULL,NUM_THREADS);
        bfs_pt(g,0,action2,NUM_THREADS);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        std::cout<<duration.count()<<std::endl;
        for(int i=0;i<NODES;i++)
            if(outputs1[i]!=outputs2[i]){
                std::cout<<"ERROR: "<<outputs1[i]<<" != "<<outputs2[i]<<std::endl;
                exit(1);
            }
    }
    return 0;
}