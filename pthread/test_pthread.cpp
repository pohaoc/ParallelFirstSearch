
#include "pthread_bfs.h"
#include "../graph_gen/graph_generator.h"
#include <math.h>
#include <chrono>

#define NUM_THREADS 10
using namespace std::chrono;
void action(int n){
    std::cout<<"thread 0x"<<std::hex<<pthread_self()<<" visited node "<<n<<std::endl;
}
void do_math(int n){
    float x =(sqrt((double)n * (float)(pthread_self()) + 1e-15));
    x *=pow(x,1.1/100);
    std::cout<<x;
}
int main(){
    adjacencyList g = generate_graph_list(100,2,70,42);
    print_graph(g);
    std::cout<<"========serial bfs========\n";
    auto start = high_resolution_clock::now();
    bfs(g,0,do_math);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout<<"time: "<<duration.count()<<std::endl;
    std::cout<<"========pthread bfs========\n";
    start = high_resolution_clock::now();
    bfs_pt(g,0,do_math,NUM_THREADS);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout<<"time: "<<duration.count()<<std::endl;
    

    // bfs(g,0,do_math);
    // std::cout<<std::endl;
    // bfs_pt(g,0,do_math,NUM_THREADS);
    // std::cout<<std::endl;
    return 0;
}