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

public:
    State(std::vector <Color> vertex_colors, std::vector<bool> selected_edges, int total_weight)
            : vertex_colors_(std::move(vertex_colors)), selected_edges_(std::move(selected_edges)),
              total_weight_(total_weight) {}

    void select_edge(unsigned int idx) {
        if (idx >= selected_edges_.size())
            throw std::out_of_range("Edge index out of range");

        selected_edges_.at(idx) = true;
    }

    void unselect_edge(unsigned int idx) {
        if (idx >= selected_edges_.size())
            throw std::out_of_range("Edge index out of range");

        selected_edges_.at(idx) = false;
    }

    void color_vertex(int idx, Color color) {
        vertex_colors_.at(idx) = color;
    }
};

#endif //MBPSEQUENTIAL_STATE_H
