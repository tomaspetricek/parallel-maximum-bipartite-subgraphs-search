#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "Graph.h"
#include "Finder.h"
#include "read.h"


int main() {
    std::filesystem::path path{"../../graf_mbp/graf_10_3.txt"};
    Graph graph = read_graph(path);

    std::cout << graph.n_edges() << std::endl;

    Finder finder{graph};
    finder.find();

    return 0;
}
