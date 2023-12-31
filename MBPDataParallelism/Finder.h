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
    State best_;
    EdgeListGraph graph_;
    long recursion_called_ = 0;
    std::unique_ptr<Explorer> expl_;

public:
    explicit Finder(EdgeListGraph graph, std::unique_ptr<Explorer> expl)
            :graph_(std::move(graph)),
             best_(graph.n_vertices(), graph.n_edges()),
             expl_(std::move(expl)) { }

    // DFS without B&B has complexity: O(3^n), where n is the number of edges.
    // There are 3 options for each edge: without, with 1st coloring order
    // and with 2nd coloring order.
    void bb_dfs(State curr, Explorer* expl = nullptr) {
        #pragma omp atomic update
        recursion_called_++;

        if (curr.n_colored()==graph_.n_vertices() && curr.subgraph_connected()
                && best_.total_weight()<curr.total_weight())
            #pragma omp critical
            best_ = curr;

        for (int edge_idx = curr.start_edge_idx(); edge_idx<graph_.n_edges(); edge_idx++) {
            if (expl)
                if (!expl->keep_exploring(curr))
                    return;

            // check upper bound
            if (curr.total_weight()+(graph_.total_weight()-curr.potential_weight())
                    <best_.total_weight())
                return;

            // update curr_state
            curr.potential_weight_ += graph_.edge(edge_idx).weight;
            curr.start_edge_idx_++;

            select_edge(Green, Red, curr, edge_idx, expl);

            select_edge(Red, Green, curr, edge_idx, expl);
        }
    }

    void select_edge(Color from, Color to, State curr, int edge_idx, Explorer* expl = nullptr) {
        Edge edge = graph_.edge(edge_idx);

        Color curr_from = curr.vertex_color(edge.vert_from);
        Color curr_to = curr.vertex_color(edge.vert_to);

        if ((curr_from==from || curr_from==Colorless) && (curr_to==to || curr_to==Colorless)) {
            // change colors
            curr.vertex_color(edge.vert_from, from);
            curr.vertex_color(edge.vert_to, to);

            // select edge
            curr.select_edge(edge_idx, edge);

            bb_dfs(curr, expl);
        }
    }

    // Coloring the starting vertex ensures that there is only one way (direction)
    // to color the graph and therefore eliminates half of the possible solutions.
    State find() {
        graph_.sort_edges();

        // color start vertex
        best_.vertex_color(0, Red);

        // prepare states
        bb_dfs(best_, expl_.get());
        std::vector<State> states = expl_->states();

        // find best state
        #pragma omp parallel for
        for (int i = 0; i<states.size(); i++) {
            bb_dfs(states[i]);
        }

        return best_;
    }

    long recursion_called() const {
        return recursion_called_;
    }
};

#endif //MBPSEQUENTIAL_FINDER_H
