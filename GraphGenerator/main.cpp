#include <iostream>
#include "command.h"


int main()
{
    generator gen;
    gen.graph_type(generator::ad);
    gen.n_vertices(200);
    gen.degree_edges(13);
    gen.connected_required();
    gen.weights_interval(80, 120);

    std::string filename;

    for (int i{0}; i < 10; i++) {
        filename = "text_" + std::to_string(i) + ".txt";
        gen.output_filename(filename);
        std::cout << gen.str() << ";" << std::endl;
    }

    return 0;
}