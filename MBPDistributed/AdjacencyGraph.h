//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_ADJACENCYLISTGRAPH_H
#define MBPSEQUENTIAL_ADJACENCYLISTGRAPH_H

class AdjacencyGraph {
    std::vector<std::vector<int>> adj_list_;
    int n_vertices_;    // make constant
    int n_edges_;

    void dfs(std::vector<bool> &visited, size_t &n_visited, int vert_from = 0) const {
        visited.at(vert_from) = true;
        n_visited++;

        for (const auto &vert_to : adj_list_[vert_from])
            if (!visited[vert_to])
                dfs(visited, n_visited, vert_to);
    }

public:
    explicit AdjacencyGraph(int n_vertices)
            : adj_list_(n_vertices), n_vertices_(n_vertices), n_edges_(0) {}

    const std::vector<int> &neighbors(int vert_idx) const {
        return adj_list_[vert_idx];
    }

    void add_edge(int vert_from, int vert_to) {
        if (vert_from >= n_vertices_ || vert_to >= n_vertices_)
            throw std::out_of_range("Vertex index out of range");

        adj_list_.at(vert_from).emplace_back(vert_to);
        adj_list_.at(vert_to).emplace_back(vert_from);
        n_edges_++;
    }

    bool is_connected() const {
        std::vector<bool> visited(n_vertices_, false);
        size_t n_visited{0};

        dfs(visited, n_visited);

        return n_visited == visited.size();
    }

    int n_edges() const {
        return n_edges_;
    }
};

#endif //MBPSEQUENTIAL_ADJACENCYLISTGRAPH_H
