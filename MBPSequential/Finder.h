//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_FINDER_H
#define MBPSEQUENTIAL_FINDER_H

#include <utility>
#include "Graph.h"
#include "State.h"


class Finder {
    State best_state_;
    const Graph graph_;

public:
    explicit Finder(Graph graph)
            : graph_(std::move(graph)),
              best_state_(std::vector<Color>(graph.n_vertices(), Colorless), std::vector<bool>(graph.n_edges(), false),
                          0) {}

    void bb_dfs(State curr_state, int start_edge_idx = 0) {
        for (int edge_idx{start_edge_idx}; edge_idx < graph_.n_edges(); edge_idx++) {
            curr_state.select_edge(edge_idx);

            bb_dfs(curr_state, edge_idx + 1);

            curr_state.unselect_edge(edge_idx);
        }
    }

    State find() {
        bb_dfs(best_state_);

        return best_state_;
    }
};


#endif //MBPSEQUENTIAL_FINDER_H
