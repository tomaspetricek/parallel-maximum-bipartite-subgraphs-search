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

    void add_edge(unsigned int vert1, unsigned int vert2, unsigned int weight) {
        if (vert1 >= n_vertices_ || vert2 >= n_vertices_) {
            throw std::out_of_range("Vertex label out of range");
        }

        edges_.emplace_back(vert1, vert2);
        weights_.emplace_back(weight);
        n_edges_++;
    }

    int n_vertices() const {
        return n_vertices_;
    }

    int n_edges() const {
        return edges_.size();
    }
};


#endif //MBPSEQUENTIAL_GRAPH_H
