#include <iostream>
#include <mpi.h>


// To run on 4 cores use command:
// mpirun -hostfile /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/TryMPI/hosts /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/TryMPI/cmake-build-debug/TryMPI
int main()
{
    // Initialize the MPI environment
    MPI_Init(nullptr, nullptr);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
            processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}
