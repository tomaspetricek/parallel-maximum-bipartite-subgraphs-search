//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_STATE_H
#define MBPSEQUENTIAL_STATE_H

#include "Color.h"


class State {
    std::vector <Color> vertex_colors_;
    std::vector<bool> selected_edges_;
    int total_weight_;
    int n_selected_;

public:
    explicit State(std::vector <Color> vertex_colors, std::vector<bool> selected_edges, int total_weight, int n_selected)
            : vertex_colors_(std::move(vertex_colors)), selected_edges_(std::move(selected_edges)),
              total_weight_(total_weight), n_selected_(n_selected) {}

    void select_edge(unsigned int idx) {
        if (idx >= selected_edges_.size())
            throw std::out_of_range("Edge index out of range");

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
        vertex_colors_.at(idx) = color;
    }

    Color vertex_color(int idx) {
        return vertex_colors_.at(idx);
    }

    int n_selected() const {
        return n_selected_;
    }
};

#endif //MBPSEQUENTIAL_STATE_H
