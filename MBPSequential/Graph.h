//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_GRAPH_H
#define MBPSEQUENTIAL_GRAPH_H

#include <utility>
#include <vector>


class Graph {
    const int n_vertices_;
    int n_edges_;
    std::vector<std::pair<int, int>> edges_;
    std::vector<int> weights_;

public:
    explicit Graph(const int n_vertices)
            : n_vertices_(n_vertices), n_edges_(0) {}

    void add_edge(unsigned int vert1_idx, unsigned int vert2_idx, unsigned int weight) {
        if (vert1_idx >= n_vertices_ || vert2_idx >= n_vertices_)
            throw std::out_of_range("Vertex index out of range");

        edges_.emplace_back(vert1_idx, vert2_idx);
        weights_.emplace_back(weight);
        n_edges_++;
    }

    int n_vertices() const {
        return n_vertices_;
    }

    int n_edges() const {
        return edges_.size();
    }

    std::pair<int, int> edge(unsigned int idx) const {
        if (idx >= edges_.size())
            throw std::out_of_range("Edge index out of range");

        return edges_.at(idx);
    }

    int weight(unsigned int idx) const {
        if (idx >= weights_.size())
            throw std::out_of_range("Weights index out of range");

        return weights_.at(idx);
    }
};


#endif //MBPSEQUENTIAL_GRAPH_H
