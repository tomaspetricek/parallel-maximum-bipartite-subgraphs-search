#include <filesystem>
#include <chrono>
#include <functional>

#include <boost/mpi.hpp>
#include <ostream>

#include "finder.h"
#include "read.h"
#include "master.h"
#include "slave.h"
#include "args.h"
#include "table.h"
#include "csv.h"

struct result {
    pdp::finder::state best;
    double duration;

    result(pdp::finder::state best, double duration)
            :best(std::move(best)), duration(duration) { }

    friend std::ostream& operator<<(std::ostream& os, const result& result)
    {
        os << std::endl
           << "duration: " << result.duration << std::endl
           << "best: " << result.best;
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

void run_sequential(const std::filesystem::path& graph_path, const std::filesystem::path& csv_path)
{
    auto graph = read_graph(graph_path);

    std::cout << "filename: " << graph_path.filename() << std::endl
              << "n vertices: " << graph.n_vertices() << std::endl
              << "n edges: " << graph.n_edges() << std::endl;

    // prepare table
    pdp::table<std::string, int, int, double> table{
            {"filename", "n vertices", "n edges", "time[s]"}
    };

    auto finder = pdp::finder::sequential(graph);
    auto root = pdp::finder::state(graph.n_vertices(), graph.n_edges());
    auto res = measure_duration([&] { return finder.find(root); });

    std::cout << "res: " << res << std::endl;

    // save results to csv
    table.add_row({graph_path.filename(), graph.n_vertices(), graph.n_edges(), res.duration});
    pdp::csv csv{csv_path};
    csv.write(table);
}

void run_data_parallel(const std::filesystem::path& graph_path, const std::filesystem::path& csv_path,
        int max_depth, int n_threads)
{
    omp_set_num_threads(n_threads);
    assert((n_threads==omp_get_max_threads(), "Number of set threads is not equal to maximum number of threads"));

    auto graph = read_graph(graph_path);

    std::cout << "filename: " << graph_path.filename() << std::endl
              << "n vertices: " << graph.n_vertices() << std::endl
              << "n edges: " << graph.n_edges() << std::endl
              << "max depth: " << max_depth << std::endl
              << "n threads: " << n_threads << std::endl;

    // prepare table
    pdp::table<std::string, int, int, int, int, double> table{
            {"filename", "n vertices", "n edges", "max depth", "n threads", "time[s]"}
    };

    auto explorer = pdp::finder::explorer(graph.n_edges(), max_depth);
    auto finder = pdp::finder::data_parallel(graph, explorer);
    auto root = pdp::finder::state(graph.n_vertices(), graph.n_edges());
    auto res = measure_duration([&] { return finder.find(root); });

    std::cout << "res: " << res << std::endl;

    // save results to csv
    table.add_row({graph_path.filename(), graph.n_vertices(), graph.n_edges(), max_depth, n_threads, res.duration});
    pdp::csv csv{csv_path};
    csv.write(table);
}

void run_task_parallel(const std::filesystem::path& graph_path, const std::filesystem::path& csv_path,
        float seq_ratio, int n_threads)
{
    omp_set_num_threads(n_threads);
    assert((n_threads==omp_get_max_threads(), "Number of set threads is not equal to maximum number of threads"));

    auto graph = read_graph(graph_path);

    std::cout << "filename: " << graph_path.filename() << std::endl
              << "n vertices: " << graph.n_vertices() << std::endl
              << "n edges: " << graph.n_edges() << std::endl
              << "sequential ratio: " << seq_ratio << std::endl
              << "n threads: " << n_threads << std::endl;

    // prepare table
    pdp::table<std::string, int, int, float, int, double> table{
            {"filename", "n vertices", "n edges", "sequential ratio", "n threads", "time[s]"}
    };

    auto finder = pdp::finder::task_parallel(graph, seq_ratio);
    auto root = pdp::finder::state(graph.n_vertices(), graph.n_edges());
    auto res = measure_duration([&] { return finder.find(root); });

    std::cout << "res: " << res << std::endl;

    // save results to csv
    table.add_row({graph_path.filename(), graph.n_vertices(), graph.n_edges(), seq_ratio, n_threads, res.duration});
    pdp::csv csv{csv_path};
    csv.write(table);
}

void run_distributed(const std::filesystem::path& graph_path, const std::filesystem::path& csv_path,
        int max_depth_master, int max_depth_slave, int n_threads)
{
    boost::mpi::environment env;
    boost::mpi::communicator world;

    omp_set_num_threads(n_threads);
    assert((n_threads==omp_get_max_threads(), "Number of set threads is not equal to maximum number of threads"));

    if (world.size()==1)
        throw std::runtime_error("Cannot be distributed. Single process is running.");

    if (world.rank()==pdp::process::rank::master) {
        auto graph = read_graph(graph_path);
        int n_procs = world.size();

        std::cout << "filename: " << graph_path.filename() << std::endl
                  << "n vertices: " << graph.n_vertices() << std::endl
                  << "n edges: " << graph.n_edges() << std::endl
                  << "n processes: " << n_procs << std::endl
                  << "max depth master: " << max_depth_master << std::endl
                  << "max depth slave: " << max_depth_slave << std::endl
                  << "n threads: " << n_threads << std::endl;

        // prepare table
        pdp::table<std::string, int, int, int, int, int, int, double> table{
                {"filename", "n vertices", "n edges", "max depth master", "max depth slave", "n processes", "n threads", "time[s]"}
        };

        auto master_explorer = pdp::finder::explorer(graph.n_edges(), max_depth_master);
        auto slave_explorer = pdp::finder::explorer(graph.n_edges(), max_depth_master+max_depth_slave);
        pdp::process::master proc = pdp::process::master(world, graph, master_explorer, slave_explorer);
        auto res = measure_duration([&] { return proc.start(); });

        std::cout << "res: " << res << std::endl;

        // save results to csv
        table.add_row({graph_path.filename(), graph.n_vertices(), graph.n_edges(), max_depth_master, max_depth_slave,
                       n_procs, n_threads, res.duration});
        pdp::csv csv{csv_path};
        csv.write(table);
    }
    else {
        pdp::process::slave proc = pdp::process::slave(world);
        proc.start();
    }
}

int main(int argc, char* argv[])
{
    auto args = pdp::args::parse(argc, argv);
    std::filesystem::path graph_path(args.get("f"));
    std::filesystem::path csv_path(args.get("o"));
    std::string mode{args.get("m")};

    if (mode=="SEQ") {
        run_sequential(graph_path, csv_path);
    }
    else if (mode=="TASK") {
        float seq_ratio = std::stof(args.get("r"));
        int n_threads = std::stoi(args.get("t"));
        run_task_parallel(graph_path, csv_path, seq_ratio, n_threads);
    }
    else if (mode=="DATA") {
        int max_depth = std::stoi(args.get("d"));
        int n_threads = std::stoi(args.get("t"));
        run_data_parallel(graph_path, csv_path, max_depth, n_threads);
    }
    else if (mode=="DISTRIB") {
        int max_depth_master = std::stoi(args.get("dm"));
        int max_depth_slave = std::stoi(args.get("ds"));
        int n_threads = std::stoi(args.get("t"));
        run_distributed(graph_path, csv_path, max_depth_master, max_depth_slave, n_threads);
    }
    else {
        throw std::runtime_error("Mode not supported");
    }

    return EXIT_SUCCESS;
}