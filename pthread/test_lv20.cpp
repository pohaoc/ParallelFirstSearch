
#include "pthread_bfs.h"
#include "../graph_gen/graph_generator.h"
#include <math.h>
#include <chrono>
using namespace std::chrono;
#define NUM_THREADS 28
#define NODES 1048576

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
    adjacencyList g = generate_graph_list_from_text();
    // print_graph(g);
    if(g.size()!=NODES){
        std::cout<<"ERROR!\n";
        exit(1);
    }
    for(int i=0;i<NODES+1;i++){
        inputs[i]=i;
        outputs1[i]=1;
        outputs2[i]=1;
    }
    inputs[0]=1;
    std::cout<<"========serial bfs no action========\n";
    auto start = high_resolution_clock::now();
    bfs(g,0,NULL);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout<<"time (ms): "<<duration.count()<<std::endl;
    std::cout<<"========pthread bfs no action========\n";
    start = high_resolution_clock::now();
    bfs_pt(g,0,NULL,NUM_THREADS);
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    std::cout<<"time (ms): "<<duration.count()<<std::endl;


    std::cout<<"========serial bfs with factorial calculation========\n";
    start = high_resolution_clock::now();
    bfs(g,0,action1);
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    std::cout<<"time (ms): "<<duration.count()<<std::endl;
    std::cout<<"========pthread bfs with factorial calculation========\n";
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
    for(node i = NODES;i>=0;i--)
        if(outputs2[i]!=1&& !isinf(outputs2[i])){

            std::cout<<i<<": "<<outputs2[i]<<std::endl;
            break;
        }
    
    return 0;
}