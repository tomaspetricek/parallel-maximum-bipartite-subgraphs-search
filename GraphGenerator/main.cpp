#include <iostream>
#include <fstream>
#include "command.h"


int main()
{
    generator gen;
    gen.connected_required();
    gen.graph_type(generator::ad);
    gen.weights_interval(80, 120);

    std::ofstream filenames{"../filenames.txt", std::ios_base::app},
            commands{"../commands.txt", std::ios_base::app};

    std::string filename;

    for (int n{22}; n < 50; n++) {
        for (int k{3}; k<=n/2 && k<=5; k++) {
            std::stringstream ss;
            ss << n << "-" << k << ".txt";
            filename = ss.str();

            gen.n_vertices(n);
            gen.degree_edges(k);
            gen.output_filename(filename);

            filenames << filename << std::endl;
            commands << gen.str() << ";" << std::endl;
        }
    }

    return 0;
}