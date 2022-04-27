#include "graph_generator.h"

int main(){
    auto g1 = generate_graph_list(10,3,6,42);
    print_graph(g1);
    auto g2 = generate_graph_matrix(10,3,6,42);
    print_graph(g2);
    return 0;
}
