#include <filesystem>
#include <map>
#include <chrono>
#include <functional>

#include <boost/mpi.hpp>
#include <ostream>

#include "finder.h"
#include "read.h"
#include "master.h"
#include "slave.h"
#include "args.h"

struct result {
    pdp::finder::state best;
    double duration;

    result(pdp::finder::state best, double duration)
            :best(std::move(best)), duration(duration) { }

    friend std::ostream& operator<<(std::ostream& os, const result& result)
    {
        os << "duration: " << result.duration << std::endl << "best: " << std::endl << result.best;
        return os;
    }
};

result measure_duration(const std::function<pdp::finder::state()>& find)
{
    auto begin = std::chrono::high_resolution_clock::now();

    auto best = find();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);

    return result(best, duration.count()*1e-9);
}

void run_distributed(const std::filesystem::path& path, int max_depth_master, int max_depth_slave)
{
    boost::mpi::environment env;
    boost::mpi::communicator world;

    if (world.size()==1)
        throw std::runtime_error("Cannot be distributed. Single process is running.");

    if (world.rank()==pdp::process::rank::master) {
        auto graph = read_graph(path);

        std::cout << "n edges: " << graph.n_edges() << std::endl
                  << "n vertices: " << graph.n_vertices() << std::endl
                  << "n processes: " << world.size() << std::endl
                  << "max depth master: " << max_depth_master << std::endl
                  << "max depth slave: " << max_depth_slave << std::endl;

        auto master_explorer = pdp::finder::explorer(graph.n_edges(), max_depth_master);
        auto slave_explorer = pdp::finder::explorer(graph.n_edges(), max_depth_master+max_depth_slave);

        pdp::process::master proc = pdp::process::master(world, graph, master_explorer, slave_explorer);
        auto res = measure_duration([&] { return proc.start(); });

        std::cout << res << std::endl;
    }
    else {
        pdp::process::slave proc = pdp::process::slave(world);
        proc.start();
    }
}

void run_sequential(const std::filesystem::path& path)
{
    auto graph = read_graph(path);

    std::cout << "n edges: " << graph.n_edges() << std::endl
              << "n vertices: " << graph.n_vertices() << std::endl;

    auto finder = pdp::finder::sequential(graph);
    auto root = pdp::finder::state(graph.n_vertices(), graph.n_edges());
    auto res = measure_duration([&] { return finder.find(root); });

    std::cout << res << std::endl;
}

void run_data_parallel(const std::filesystem::path& path, int max_depth)
{
    auto graph = read_graph(path);

    std::cout << "n edges: " << graph.n_edges() << std::endl
              << "n vertices: " << graph.n_vertices() << std::endl
              << "max depth: " << max_depth << std::endl;

    auto explorer = pdp::finder::explorer(graph.n_edges(), max_depth);
    auto finder = pdp::finder::data_parallel(graph, explorer);
    auto root = pdp::finder::state(graph.n_vertices(), graph.n_edges());
    auto res = measure_duration([&] { return finder.find(root); });

    std::cout << res << std::endl;
}

void run_task_parallel(const std::filesystem::path& path, float seq_ratio)
{
    auto graph = read_graph(path);

    std::cout << "n edges: " << graph.n_edges() << std::endl
              << "n vertices: " << graph.n_vertices() << std::endl
              << "sequential ratio: " << seq_ratio << std::endl;

    auto finder = pdp::finder::task_parallel(graph, seq_ratio);
    auto root = pdp::finder::state(graph.n_vertices(), graph.n_edges());
    auto res = measure_duration([&] { return finder.find(root); });

    std::cout << res << std::endl;
}

int main(int argc, char* argv[])
{
    auto args = pdp::args::parse(argc, argv);
    std::filesystem::path path(args.get("f"));
    std::string mode{args.get("m")};

    std::cout << "filename: " << path.filename() << std::endl
              << "mode: " << mode << std::endl;

    if (mode=="SEQ") {
        run_sequential(path);
    }
    else if (mode=="TASK") {
        float seq_ratio = std::stof(args.get("r"));
        run_task_parallel(path, seq_ratio);
    }
    else if (mode=="DATA") {
        int max_depth = std::stoi(args.get("d"));
        run_data_parallel(path, max_depth);
    }
    else if (mode=="DISTRIB") {
        int max_depth_master = std::stoi(args.get("dm"));
        int max_depth_slave = std::stoi(args.get("ds"));
        run_distributed(path, max_depth_master, max_depth_slave);
    }
    else {
        throw std::runtime_error("Mode not supported");
    }

    return EXIT_SUCCESS;
}