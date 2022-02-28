#include <filesystem>

#include "EdgeListGraph.h"
#include "Finder.h"
#include "read.h"
#include <ctime>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>

void print_state(const State& state) {
    std::cout << "Selected edges: " << to_string(state.selected_edges()) << "\n"
              << "Vertex colors: " << to_string(state.vertex_colors()) << "\n"
              << "Total weight: " << state.total_weight() << "\n"
              << "Best state n edges: " << state.subgraph_n_edges() << std::endl;
}

void test_graph(const EdgeListGraph& graph, float sequential_ratio) {
    boost::timer::cpu_timer timer;

    Finder finder{graph, sequential_ratio};
    State best_state = finder.find();

    boost::timer::cpu_times elapsed = timer.elapsed();
    auto elapsed_cpu_time(elapsed.wall);
    boost::chrono::duration<double> wall_time = boost::chrono::nanoseconds(elapsed_cpu_time);

    // print results
    print_state(best_state);
    std::cout << "Time elapsed: " << std::setprecision(3) << wall_time.count() << "\n";
    std::cout << std::setfill('-') << std::setw(50) << "" << std::setfill(' ') << std::endl;
}

void test_small_graph(float sequential_ratio) {
    EdgeListGraph graph(4);
    graph.add_edge(Edge(0, 1, 5));
    graph.add_edge(Edge(1, 3, 6));
    graph.add_edge(Edge(0, 2, 4));
    graph.add_edge(Edge(1, 2, 9));
    graph.add_edge(Edge(0, 3, 8));

    test_graph(graph, sequential_ratio);
}

void test_graphs(float sequential_ratio) {
    std::vector<std::string> filenames{
            "graf_10_3.txt",
            "graf_10_5.txt",
            "graf_10_6.txt",
            "graf_10_7.txt",
//
            "graf_12_3.txt",
            "graf_12_5.txt",
            "graf_12_6.txt",
//            "graf_12_9.txt",

            "graf_15_4.txt",
            "graf_15_5.txt",
            "graf_15_6.txt",
//            "graf_15_8.txt"
    };

    std::filesystem::path dirname{"../../graf_mbp"};

    for (const auto &filename : filenames) {
        std::cout << "Filename: " << filename << std::endl;

        EdgeListGraph graph = read_graph(dirname / filename);

        test_graph(graph, sequential_ratio);
    }
}

void test_threshold(const std::filesystem::path& path) {
    EdgeListGraph graph = read_graph(path);

    for (int i{0}; i <= 10; i++){
        std::cout << "Threshold: " << i << std::endl;
        test_graph(graph, static_cast<float>(i) / 10);
    }

    std::cout << "N edges: " << graph.n_edges() << std::endl;
}

int main() {
    test_graphs(0.65);
    return 0;
}