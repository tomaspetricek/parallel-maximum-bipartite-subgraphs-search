//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_STATE_H
#define MBPSEQUENTIAL_STATE_H

#include <ostream>
#include "Color.h"
#include "utils.h"
#include "AdjacencyGraph.h"

class Finder;


class State {
    std::vector<Color> vertex_colors_;
    std::vector<bool> selected_edges_;
    AdjacencyGraph subgraph_;
    int total_weight_;
    int n_selected_;
    int n_colored_;
    bool subgraph_connected_;
    int potential_weight_;
    int start_edge_idx_;

    friend class Finder;

public:
    explicit State(int n_vertices, int n_edges)
            :vertex_colors_(std::vector<Color>(n_vertices, Colorless)),
             selected_edges_(std::vector<bool>(n_edges, false)),
             total_weight_(0), n_selected_(0), n_colored_(0),
             subgraph_(n_vertices), subgraph_connected_(false),
             potential_weight_(0), start_edge_idx_(0){}

    void select_edge(unsigned int idx, const Edge& edge) {
        if (idx >= selected_edges_.size())
            throw std::out_of_range("Edge index out of range");

        // add edge only if needed
        if (!subgraph_connected_)
            subgraph_.add_edge(edge.vert_from, edge.vert_to);

        total_weight_ += edge.weight;

        // select edge
        selected_edges_.at(idx) = true;
        n_selected_++;
    }

    void vertex_color(int idx, Color color) {
        if (vertex_colors_.at(idx) == Colorless && color != Colorless) {
            n_colored_++;
        } else if (vertex_colors_.at(idx) != Colorless && color == Colorless) {
            n_colored_--;
        }

        vertex_colors_.at(idx) = color;
    }

    Color vertex_color(int idx) {
        return vertex_colors_.at(idx);
    }

    int n_selected() const {
        return n_selected_;
    }

    int n_colored() const {
        return n_colored_;
    }

    const std::vector<Color> &vertex_colors() const {
        return vertex_colors_;
    }

    const std::vector<bool> &selected_edges() const {
        return selected_edges_;
    }

    bool subgraph_connected() {
        if (subgraph_connected_) {
            return true;
        // Once the subgraph has been connected all its children
        // are connected.
        } else if (subgraph_.is_connected()) {
            subgraph_connected_ = true;
            return true;
        } else {
            return false;
        }
    }

    int subgraph_n_edges() const {
        return subgraph_.n_edges();
    }

    int total_weight() const {
        return total_weight_;
    }

    int potential_weight() const {
        return potential_weight_;
    }

    int start_edge_idx() const {
        return start_edge_idx_;
    }

    friend std::ostream& operator<<(std::ostream& os, const State& state);
};

std::ostream& operator<<(std::ostream& os, const State& state) {
    os << "Total weight: " << state.total_weight_ << std::endl
       << "Vertex colors: " << to_string(state.vertex_colors_) << std::endl
       << "Selected edges: " << to_string(state.selected_edges_);

    return os;
}

#endif //MBPSEQUENTIAL_STATE_H
