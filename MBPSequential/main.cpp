#include <filesystem>

#include "EdgeListGraph.h"
#include "Finder.h"
#include "read.h"
#include <time.h>


int main() {
    std::filesystem::path path{"../../graf_mbp/graf_15_8.txt"};
    EdgeListGraph graph = read_graph(path);

    /*
    EdgeListGraph graph(4);
    graph.add_edge(Edge(0, 1, 5));
    graph.add_edge(Edge(1, 3, 6));
    graph.add_edge(Edge(0, 2, 4));
    graph.add_edge(Edge(1, 2, 9));
    graph.add_edge(Edge(0, 3, 8));
    */

    clock_t start = clock();

    Finder finder{graph};
    finder.find();

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}
