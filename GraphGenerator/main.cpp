#include <iostream>
#include <fstream>
#include "command.h"


int main()
{
    int n_vertices{15};
    generator gen;
    gen.connected_required();
    gen.graph_type(generator::ad);
    gen.n_vertices(n_vertices);

    gen.weights_interval(80, 120);

    std::ofstream filenames{"../filenames.txt", std::ios_base::app},
            commands{"../commands.txt", std::ios_base::app};

    std::string filename;

    for (int k{3}; k<=n_vertices/2; k++) {
        std::stringstream ss;
        ss << n_vertices << "-" << k << ".txt";
        filename = ss.str();

        gen.degree_edges(k);
        gen.output_filename(filename);

        filenames << filename << std::endl;
        commands << gen.str() << ";" << std::endl;
    }

    return 0;
}