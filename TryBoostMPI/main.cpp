#include <boost/mpi.hpp>
#include <iostream>
#include <boost/serialization/string.hpp>
namespace mpi = boost::mpi;

// To run use command:
//mpirun -np 2 /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/TryBoostMPI/cmake-build-debug/TryBoostMPI
int main(int argc, char* argv[])
{
    mpi::environment env(argc, argv);
    mpi::communicator world;

    if (world.rank() == 0) {
        world.send(1, 0, std::string("Hello"));
        std::string msg;
        world.recv(1, 1, msg);
        std::cout << msg << "!" << std::endl;
    } else {
        std::string msg;
        world.recv(0, 0, msg);
        std::cout << msg << ", ";
        std::cout.flush();
        world.send(0, 1, std::string("world"));
    }
    return 0;
}

