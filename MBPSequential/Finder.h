//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_FINDER_H
#define MBPSEQUENTIAL_FINDER_H


#include <utility>
#include "Graph.h"


class Finder {
    const int red{0};
    const int green{1};
    const int colorless{2};
    const int best_total_weight{0};
    const Graph graph;

public:
    explicit Finder(Graph graph) : graph(std::move(graph)) {}

    void find() {
        std::vector<int> vertex_colors(graph.n_edges());
        std::vector<bool> selected_edges(graph.n_vertices());

        std::cout << "Done for today, Looking forward to tomorrow!" << std::endl;
    }
};


/*
class Finder {
    const int red{0};
    const int green{1};
    const int colorless{2};
    const int best_total_weight{0};
    Graph graph;

public:
    explicit Finder(Graph graph) : graph(std::move(graph)) {}

    void find() {
        std::array<int, static_cast<int>(graph.n_vertices)> vertex_colors;
        std::array<bool, static_cast<int>(graph.n_edges)> selected_edges{false};

        std::cout << "Hello" << std::endl;

    }
};
*/

#endif //MBPSEQUENTIAL_FINDER_H
