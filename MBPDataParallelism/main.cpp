#include <filesystem>

#include "EdgeListGraph.h"
#include "Finder.h"
#include "read.h"
#include <map>
#include <chrono>
#include <utility>

struct Result {
    State best;
    double duration;

    Result(State best, double duration)
            :best(std::move(best)), duration(duration) { }
};

Result measure_duration(Finder& finder)
{
    auto begin = std::chrono::high_resolution_clock::now();

    State best = finder.find();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);

    return Result(best, duration.count()*1e-9);
}

EdgeListGraph get_small_graph()
{
    EdgeListGraph graph(4);
    graph.add_edge(Edge(0, 1, 5));
    graph.add_edge(Edge(1, 3, 6));
    graph.add_edge(Edge(0, 2, 4));
    graph.add_edge(Edge(1, 2, 9));
    graph.add_edge(Edge(0, 3, 8));

    return graph;
}

std::vector<std::filesystem::path> get_graphs_filenames()
{
    std::vector<std::filesystem::path> filenames{
            "graf_10_3.txt",
            "graf_10_5.txt",
            "graf_10_6.txt",
            "graf_10_7.txt",

            "graf_12_3.txt",
            "graf_12_5.txt",
            "graf_12_6.txt",
//            "graf_12_9.txt",

            "graf_15_4.txt",
            "graf_15_5.txt",
            "graf_15_6.txt",
//            "graf_15_8.txt"
    };

    return filenames;
}

std::map<std::string, std::string> parse_args(int argc, char* argv[])
{
    std::map<std::string, std::string> args;
    std::string val;
    std::string opt;

    for (int i{1}; i<argc; i++) {
        opt = argv[i];
        std::erase(opt, '-');
        val = argv[++i];
        args[opt] = val;
    }

    return args;
}

void test_graph(const EdgeListGraph& graph, int max_idx)
{
    std::unique_ptr<Explorer> expl = std::make_unique<Explorer>(graph.n_edges(), max_idx);
    Finder finder(graph, std::move(expl));
    auto res = measure_duration(finder);

    std::cout << "Max idx: " << max_idx << std::endl
              << "N recursions: " << finder.recursion_called() << std::endl
              << "N duration: " << res.duration << std::endl
              << "Best state: " << std::endl << res.best << std::endl;

    std::cout << std::setfill('-') << std::setw(50) << "" << std::setfill(' ') << std::endl;
}

int main(int argc, char* argv[])
{
    auto args = parse_args(argc, argv);
    std::filesystem::path path(args["f"]);
    int max_idx = std::stoi(args["m"]);

    std::cout << "Filename: " << path.filename() << std::endl;
    auto graph = read_graph(path);
    test_graph(graph, max_idx);

}