#include <filesystem>

#include "EdgeListGraph.h"
#include "Finder.h"
#include "read.h"
#include <map>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>

void print_state(const State& state) {
    std::cout << "Selected edges: " << to_string(state.selected_edges()) << "\n"
              << "Vertex colors: " << to_string(state.vertex_colors()) << "\n"
              << "Total weight: " << state.total_weight() << "\n"
              << "Best state n edges: " << state.subgraph_n_edges() << std::endl;
}

void test_graph(Finder& finder) {
    boost::timer::cpu_timer timer;

    State best_state = finder.find();

    boost::timer::cpu_times elapsed = timer.elapsed();
    auto elapsed_cpu_time(elapsed.wall);
    boost::chrono::duration<double> wall_time = boost::chrono::nanoseconds(elapsed_cpu_time);

    // print results
    print_state(best_state);
    std::cout << "Time elapsed: " << std::setprecision(3) << wall_time.count() << "\n";
    std::cout << std::setfill('-') << std::setw(50) << "" << std::setfill(' ') << std::endl;
}

EdgeListGraph get_small_graph() {
    EdgeListGraph graph(4);
    graph.add_edge(Edge(0, 1, 5));
    graph.add_edge(Edge(1, 3, 6));
    graph.add_edge(Edge(0, 2, 4));
    graph.add_edge(Edge(1, 2, 9));
    graph.add_edge(Edge(0, 3, 8));

    return graph;
}

std::vector<EdgeListGraph> get_example_graphs() {
    std::vector<std::string> filenames{
//            "graf_10_3.txt",
//            "graf_10_5.txt",
//            "graf_10_6.txt",
//            "graf_10_7.txt",
//
//            "graf_12_3.txt",
//            "graf_12_5.txt",
//            "graf_12_6.txt",
//            "graf_12_9.txt",

//            "graf_15_4.txt",
//            "graf_15_5.txt",
            "graf_15_6.txt",
//            "graf_15_8.txt"
    };

    std::vector<EdgeListGraph> graphs;
    graphs.reserve(filenames.size());

    std::filesystem::path dirname{"../../graf_mbp"};

    for (const auto& filename : filenames) {
        graphs.emplace_back(read_graph(dirname/filename));
    }

    return graphs;
}

std::map<std::string, std::string> parse_args(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    std::string val;
    std::string opt;

    for (int i{1}; i < argc; i++) {
        opt = argv[i];
        std::erase(opt, '-');
        val = argv[++i];
        args[opt] = val;
    }

    return args;
}

int main(int argc, char* argv[]) {
//    auto args = parse_args(argc, argv);
//
//    std::filesystem::path path(args["f"]);
//
//    auto graph = read_graph(path);

    auto graph = get_example_graphs()[0];

    std::unique_ptr<Explorer> expl = std::make_unique<Explorer>(graph.n_edges(), 15);
    Finder finder(graph, std::move(expl));

    test_graph(finder);
    return 0;
}