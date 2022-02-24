#include <iostream>
#include <omp.h>
//#include "/usr/local/opt/libomp/include/omp.h"
//#include <omp.h>


int main() {
    int p = omp_get_num_threads();

    std::cout << "N cores: " << p << std::endl;

    #pragma omp parallel
    {
        #pragma omp critical
        std::cout << "Hello World!" << std::endl;
    }

    return EXIT_SUCCESS;
}
