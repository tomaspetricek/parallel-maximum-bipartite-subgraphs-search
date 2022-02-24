#include <filesystem>

#include "EdgeListGraph.h"
#include "Finder.h"
#include "read.h"
#include <ctime>

EdgeListGraph test_small_graph() {
    EdgeListGraph graph(4);
    graph.add_edge(Edge(0, 1, 5));
    graph.add_edge(Edge(1, 3, 6));
    graph.add_edge(Edge(0, 2, 4));
    graph.add_edge(Edge(1, 2, 9));
    graph.add_edge(Edge(0, 3, 8));

    graph.sort_edges();

    State best_state = Finder(graph).find();

    std::cout << "N edges: " << graph.n_edges() << "\n"
              << "Selected edges: " << to_string(best_state.selected_edges()) << "\n"
              << "Vertex colors: " << to_string(best_state.vertex_colors()) << "\n"
              << "Total weight: " << best_state.total_weight() << "\n"
              << "Best state n edges: " << best_state.subgraph_n_edges() << std::endl;
    return graph;
}

void test_graphs() {
    std::vector<std::string> filenames{
            "graf_10_3.txt",
            "graf_10_5.txt",
            "graf_10_6.txt",
            "graf_10_7.txt",

            "graf_12_3.txt",
            "graf_12_5.txt",
            "graf_12_6.txt",
//            "graf_12_9.txt",
//
            "graf_15_4.txt",
            "graf_15_5.txt",
//            "graf_15_6.txt",
//            "graf_15_8.txt"
    };

    std::filesystem::path dirname{"../../graf_mbp"};

    for (const auto &filename : filenames) {
        EdgeListGraph graph = read_graph(dirname / filename);

        // sort edges
        graph.sort_edges();

        clock_t start = clock();

        Finder finder{graph};
        State best_state = finder.find();

        clock_t stop = clock();
        double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;

        std::cout << "Filename: " << filename << "\n"
                  << "N edges: " << graph.n_edges() << "\n"
                  << "Selected edges: " << to_string(best_state.selected_edges()) << "\n"
                  << "Vertex colors: " << to_string(best_state.vertex_colors()) << "\n"
                  << "Total weight: " << best_state.total_weight() << "\n"
                  << "Time elapsed: " << std::setprecision(3) << elapsed << "\n"
                  << "Best state n edges: " << best_state.subgraph_n_edges() << std::endl;

        std::cout << std::setfill('-') << std::setw(50) << "" << std::setfill(' ') << std::endl;
    }
}


int main() {
    test_graphs();
    return 0;
}