//
// Created by Tomáš Petříček on 06.03.2022.
//

#ifndef MBPTASKPARALLELISM_EXPLORER_H
#define MBPTASKPARALLELISM_EXPLORER_H

#include <vector>
#include <cmath>

#include "State.h"


class Explorer {
    std::vector<State> states_;
    unsigned int max_idx_;

    static unsigned int valid_max_idx(unsigned int n_vertices, unsigned int max_idx) {
        if (n_vertices == max_idx)
            throw std::out_of_range("Max index must be lower than n vertices");

        return max_idx;
    }

public:
    Explorer(const int n_vertices, unsigned int max_idx)
            :max_idx_(valid_max_idx(n_vertices, max_idx)) {
    }

    bool keep_exploring(const State& state) {
        if (state.start_edge_idx() == max_idx_) {
            states_.emplace_back(state);
            return false;
        } else {
            return true;
        }
    }

    std::vector<State> states() {
        return states_;
    }
};

#endif //MBPTASKPARALLELISM_EXPLORER_H
