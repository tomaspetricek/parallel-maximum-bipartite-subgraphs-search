//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_GRAPH_H
#define MBPSEQUENTIAL_GRAPH_H

#include <utility>
#include <vector>
#include "Edge.h"


class Graph {
    const int n_vertices_;
    int n_edges_;
    std::vector<Edge> edges_;
    int total_weight;

public:
    explicit Graph(const int n_vertices)
            : n_vertices_(n_vertices), n_edges_(0), total_weight(0) {}

    void add_edge(Edge edge) {
        if (edge.vert_from >= n_vertices_ || edge.vert_to >= n_vertices_)
            throw std::out_of_range("Vertex index out of range");

        edges_.emplace_back(edge);
        n_edges_++;
        total_weight += edge.weight;
    }

    int n_vertices() const {
        return n_vertices_;
    }

    int n_edges() const {
        return edges_.size();
    }

    Edge edge(unsigned int idx) const {
        if (idx >= edges_.size())
            throw std::out_of_range("Edge index out of range");

        return edges_.at(idx);
    }
};


#endif //MBPSEQUENTIAL_GRAPH_H
