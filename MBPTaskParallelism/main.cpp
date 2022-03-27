#include <filesystem>
#include <chrono>
#include <map>

#include "EdgeListGraph.h"
#include "Finder.h"
#include "read.h"

void print_state(const State& state)
{
    std::cout << "Selected edges: " << to_string(state.selected_edges()) << "\n"
              << "Vertex colors: " << to_string(state.vertex_colors()) << "\n"
              << "Total weight: " << state.total_weight() << "\n"
              << "Best state n edges: " << state.subgraph_n_edges() << std::endl;
}

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

void test_graph(Finder& finder)
{
    auto res = measure_duration(finder);

    std::cout << "N recursions: " << finder.recursion_called() << std::endl
              << "N duration: " << res.duration << std::endl
              << "Best state: " << std::endl << res.best << std::endl;

    std::cout << std::setfill('-') << std::setw(50) << "" << std::setfill(' ') << std::endl;
}

int main(int argc, char* argv[])
{
    auto args = parse_args(argc, argv);

    std::filesystem::path path(args["f"]);
    float sequential_ratio = std::stof(args["r"]);

    std::cout << "Filename: " << path.filename() << std::endl
              << "Sequential ratio: " << sequential_ratio << std::endl;

    auto graph = read_graph(path);

    Finder finder{graph, sequential_ratio};

    test_graph(finder);

    return 0;
}