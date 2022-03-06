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
#include <omp.h>

#include "EdgeListGraph.h"
#include "State.h"
#include "utils.h"
#include "Explorer.h"


class Finder {
    State best_state_;
    EdgeListGraph graph_;
    long recursion_called_ = 0;
    std::unique_ptr<Explorer> expl_;

public:
    explicit Finder(EdgeListGraph graph, std::unique_ptr<Explorer> expl)
            :graph_(std::move(graph)),
             best_state_(graph.n_vertices(), graph.n_edges()),
             expl_(std::move(expl)) {}

    // DFS without B&B has complexity: O(3^n), where n is the number of edges.
    // There are 3 options for each edge: without, with 1st coloring order
    // and with 2nd coloring order.
    void bb_dfs(State curr_state, Explorer* expl = nullptr) {
        #pragma omp atomic update
        recursion_called_++;

        if (curr_state.n_colored()==graph_.n_vertices() && curr_state.subgraph_connected()
                && best_state_.total_weight()<curr_state.total_weight())
            #pragma omp critical
            best_state_ = curr_state;

        for (int edge_idx = curr_state.start_edge_idx(); edge_idx < graph_.n_edges(); edge_idx++) {
            if (expl)
                if (!expl->keep_exploring(curr_state))
                    return;

            // check upper bound
            if (curr_state.total_weight()+(graph_.total_weight()-curr_state.potential_weight())
                    <best_state_.total_weight())
                return;

            // update curr_state
            curr_state.potential_weight_ += graph_.edge(edge_idx).weight;
            curr_state.start_edge_idx_++;

            select_edge(Green, Red, curr_state, edge_idx, expl);

            select_edge(Red, Green, curr_state, edge_idx, expl);
        }
    }

    void select_edge(Color color_from, Color color_to, State curr_state, int edge_idx, Explorer* expl = nullptr) {
        Edge curr_edge = graph_.edge(edge_idx);

        Color curr_color_from = curr_state.vertex_color(curr_edge.vert_from);
        Color curr_color_to = curr_state.vertex_color(curr_edge.vert_to);

        if ((curr_color_from==color_from || curr_color_from==Colorless)
                && (curr_color_to==color_to || curr_color_to==Colorless)) {

            // change colors
            curr_state.vertex_color(curr_edge.vert_from, color_from);
            curr_state.vertex_color(curr_edge.vert_to, color_to);

            // select edge
            curr_state.select_edge(edge_idx, curr_edge);

            bb_dfs(curr_state, expl);
        }
    }

    // Coloring the starting vertex ensures that there is only one way (direction)
    // to color the graph and therefore eliminates half of the possible solutions.
    State find() {
        graph_.sort_edges();

        // color start vertex
        best_state_.vertex_color(0, Red);

        // prepare states
        bb_dfs(best_state_, expl_.get());

        std::vector<State> states = expl_->states();

        std::cout << "N states: " << states.size() << std::endl;

        // find best state
        #pragma omp parallel for
        {
            for (int i = 0; i < states.size(); i++)
                bb_dfs(states[i]);
        }

        return best_state_;
    }

    long recursion_called() const {
        return recursion_called_;
    }
};

#endif //MBPSEQUENTIAL_FINDER_H
