//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_FINDER_H
#define MBPSEQUENTIAL_FINDER_H

#include <utility>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

#include "EdgeListGraph.h"
#include "State.h"

template<typename T>
std::string to_string(const std::vector<T> &v) {
    std::ostringstream oss;

    if (!v.empty()) {
        std::copy(v.begin(), v.end() - 1,
                  std::ostream_iterator<int>(oss, ","));

        oss << v.back();
    }

    return oss.str();
}


class Finder {
    State best_state_;
    const EdgeListGraph graph_;
    int n_states = 0;

public:
    explicit Finder(EdgeListGraph graph)
            : graph_(std::move(graph)),
              best_state_(graph.n_vertices(), graph.n_edges()) {}

    void bb_dfs(State curr_state, int start_edge_idx = 0) {
        n_states++;

        if (curr_state.n_colored() == graph_.n_vertices() && curr_state.subgraph_connected()
            && best_state_.total_weight() < curr_state.total_weight())
            best_state_ = curr_state;

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
            curr_state.select_edge(edge_idx, curr_edge);

            bb_dfs(curr_state, edge_idx + 1);
        }
    }

    State find() {
        bb_dfs(best_state_);

        return best_state_;
    }
};


#endif //MBPSEQUENTIAL_FINDER_H
