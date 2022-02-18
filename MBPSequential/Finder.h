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
        std::vector<int> vertex_colors(graph.n_edges(), colorless);
        std::vector<bool> selected_edges(graph.n_vertices(), false);


    }
};


#endif //MBPSEQUENTIAL_FINDER_H
