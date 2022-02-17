#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "Graph.h"
#include "Finder.h"


Graph read_graph(const std::filesystem::path &path) {
    std::ifstream file{path.string()};
    std::string line;
    int n_vertices{0};
    int weight{0};
    const int not_connected{0};

    if (file.is_open()) {
        // get n vertices
        std::getline(file, line);
        (std::istringstream(line)) >> n_vertices;

        // create graph
        Graph graph{n_vertices};

        // add edges
        for (int vert_from{0}; vert_from < n_vertices; vert_from++) {
            std::getline(file, line);
            std::istringstream ss(line);

            for (int vert_to{0}; vert_to < vert_from; vert_to++) {
                ss >> weight;

                // add edge
                if (weight != not_connected) {
                    graph.add_edge(vert_from, vert_to, weight);
                }
            }
        }

        return graph;
    } else {
        std::cerr << path.string() << std::endl;
        throw std::runtime_error("Could not open file");
    }
}


int main() {
    std::filesystem::path path{"../../graf_mbp/graf_10_3.txt"};
    auto graph = read_graph(path);

    std::cout << graph.n_edges() << std::endl;

    Finder finder{graph};
    finder.find();

    return 0;
}
