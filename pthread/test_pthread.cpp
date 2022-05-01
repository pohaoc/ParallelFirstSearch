
#include "pthread_bfs.h"
#include "../graph_gen/graph_generator.h"
#include <math.h>
#include <chrono>
#include "../serial/serial_bfs.h"

#define NUM_THREADS 28
#define NODES 500000

int output1[NODES]={0};
int output2[NODES]={0};

using namespace std::chrono;
void action1(bfs_node n){
    // std::cout<<"thread 0x"<<std::hex<<pthread_self()<<std::dec<<" visited node "<<n<<std::endl;
    output1[n]++;
}
void action2(bfs_node n){
    // std::cout<<"thread 0x"<<std::hex<<pthread_self()<<std::dec<<" visited node "<<n<<std::endl;
    output2[n]++;
}
void do_math(bfs_node n){
    float x =(sqrt((double)n * (float)(pthread_self()) + 1e-15));
    x *=pow(x,1.1/100);
    std::cout<<x;
}
int main(){
    adjacencyList g = generate_graph_list(NODES,2,20,42);
    // print_graph(g);
    std::cout<<"0: ";
    for(int i = 0;i<g[0].size();i++)
        std::cout<<g[0][i]<<", ";
    std::cout<<std::endl;
    std::cout<<"========serial bfs========\n";
    auto start = high_resolution_clock::now();
    bfs(g,0,action1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout<<"time: "<<duration.count()<<std::endl;


    std::cout<<"========pthread bfs========\n";
    start = high_resolution_clock::now();
    bfs_pt(g,0,action2,NUM_THREADS);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout<<"time: "<<duration.count()<<std::endl;
    
      for(int i = 0;i<NODES;i++){
        if(output1[i]!=output2[i])
            {
                std::cout<<"ERROR\n";
                std::cout<<i<<" "<<output1[i]<<" "<<output2[i]<<std::endl;
            }
    }
    // bfs(g,0,do_math);
    // std::cout<<std::endl;
    // bfs_pt(g,0,do_math,NUM_THREADS);
    // std::cout<<std::endl;
    return 0;
}