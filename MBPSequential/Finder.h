//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_FINDER_H
#define MBPSEQUENTIAL_FINDER_H

#include <utility>
#include <iostream>
#include "Graph.h"
#include "State.h"


class Finder {
    State best_state_;
    const Graph graph_;
    int n_states = 0;

public:
    explicit Finder(Graph graph)
            : graph_(std::move(graph)),
              best_state_(std::vector<Color>(graph.n_vertices(), Colorless), std::vector<bool>(graph.n_edges(), false),
                          0, 0) {}

    void bb_dfs(const State& curr_state, int start_edge_idx = 0) {
        n_states++;

        for (int edge_idx{start_edge_idx}; edge_idx < graph_.n_edges(); edge_idx++) {

            // select edge
            select_edge(Green, Red, curr_state, edge_idx);

            select_edge(Red, Green, curr_state, edge_idx);
        }
    }

    void select_edge(Color color_from, Color color_to, State curr_state, int edge_idx) {
        Edge curr_edge = graph_.edge(edge_idx);

        Color curr_color_from = curr_state.vertex_color(curr_edge.vert_from);
        Color curr_color_to = curr_state.vertex_color(curr_edge.vert_to);

        if ((curr_color_from == color_from || curr_color_from == Colorless)
            && (curr_color_to == color_to || curr_color_to == Colorless)) {

            // change colors
            curr_state.vertex_color(curr_edge.vert_from, color_from);
            curr_state.vertex_color(curr_edge.vert_to, color_to);

            // select edge
            curr_state.select_edge(edge_idx);
            bb_dfs(curr_state, edge_idx + 1);
        }
    }

    State find() {
        bb_dfs(best_state_);

        std::cout << n_states << std::endl;

        return best_state_;
    }
};


#endif //MBPSEQUENTIAL_FINDER_H
