#include <filesystem>
#include <map>
#include <chrono>
#include <utility>

#include <boost/mpi.hpp>

#include "edge_list.h"
#include "finder.h"
#include "read.h"
#include "master.h"
#include "slave.h"
#include "args.h"

struct result {
    pdp::state best;
    double duration;

    result(pdp::state best, double duration)
            :best(std::move(best)), duration(duration) { }
};

result measure_duration(pdp::process::master& proc)
{
    auto begin = std::chrono::high_resolution_clock::now();

    pdp::state best = proc.start();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);

    return result(best, duration.count()*1e-9);
}

void distribute(const std::filesystem::path& path, int max_depth_master, int max_depth_slave)
{
    boost::mpi::environment env;
    boost::mpi::communicator world;

    if (world.size()==1)
        throw std::runtime_error("Cannot be distributed. Single process is running.");

    if (world.rank()==pdp::process::rank::master) {
        std::cout << "N processes: " << world.size() << std::endl
                  << "Filename: " << path.filename() << std::endl
                  << "Max depth master: " << max_depth_master << std::endl
                  << "Max depth slave: " << max_depth_slave << std::endl;

        auto graph = read_graph(path);
        auto master_explorer = pdp::explorer(graph.n_vertices(), max_depth_master);
        auto slave_explorer = pdp::explorer(graph.n_vertices(), max_depth_master+max_depth_slave);

        pdp::process::master proc = pdp::process::master(world, graph, master_explorer, slave_explorer);
        auto res = measure_duration(proc);

        std::cout << "Duration: " << res.duration << std::endl
                  << "Best state: " << std::endl << res.best << std::endl;
    }
    else {
        pdp::process::slave proc = pdp::process::slave(world);
        proc.start();
    }
}

// Command to run
// time mpirun -np 4 /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug/MBPDistributed -f /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/graf_mbp/graf_12_9.txt
int main(int argc, char* argv[])
{
    auto args = pdp::args::parse(argc, argv);
    std::filesystem::path path(args.get("f"));
    int max_depth_master = std::stoi(args.get("mm"));
    int max_depth_slave = std::stoi(args.get("ms"));

    distribute(path, max_depth_master, max_depth_slave);
    return EXIT_SUCCESS;
}