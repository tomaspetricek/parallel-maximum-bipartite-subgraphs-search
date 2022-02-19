//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_STATE_H
#define MBPSEQUENTIAL_STATE_H

#include "Color.h"
#include "AdjencyListGraph.h"


class State {
    std::vector<Color> vertex_colors_;
    std::vector<bool> selected_edges_;
    AdjacencyListGraph subgraph_;
    int total_weight_;
    int n_selected_;
    int n_colored_;

public:
    explicit State(int n_vertices, int n_edges)
            : vertex_colors_(std::vector<Color>(n_vertices, Colorless)),
              selected_edges_(std::vector<bool>(n_edges, false)),
              total_weight_(0), n_selected_(0), n_colored_(0),
              subgraph_(n_vertices) {}

    void select_edge(unsigned int idx, const Edge& edge) {
        if (idx >= selected_edges_.size())
            throw std::out_of_range("Edge index out of range");

        // add edge
        subgraph_.add_edge(edge.vert_from, edge.vert_to);
        total_weight_ += edge.weight;

        // select edge
        selected_edges_.at(idx) = true;
        n_selected_++;
    }

    void unselect_edge(unsigned int idx) {
        if (idx >= selected_edges_.size())
            throw std::out_of_range("Edge index out of range");

        selected_edges_.at(idx) = false;
        n_selected_--;
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

    bool subgraph_connected() const {
        return subgraph_.is_connected();
    }

    int total_weight() const {
        return total_weight_;
    }
};

#endif //MBPSEQUENTIAL_STATE_H
