//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_STATE_H
#define MBPSEQUENTIAL_STATE_H

#include <ostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

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
             potential_weight_(0), start_edge_idx_(0) { }

    State() = default;

    void select_edge(unsigned int idx, const Edge& edge)
    {
        if (idx>=selected_edges_.size())
            throw std::out_of_range("Edge index out of range");

        // add edge only if needed
        if (!subgraph_connected_)
            subgraph_.add_edge(edge.vert_from, edge.vert_to);

        total_weight_ += edge.weight;

        // select edge
        selected_edges_.at(idx) = true;
        n_selected_++;
    }

    void vertex_color(int idx, Color color)
    {
        if (vertex_colors_.at(idx)==Colorless && color!=Colorless) {
            n_colored_++;
        }
        else if (vertex_colors_.at(idx)!=Colorless && color==Colorless) {
            n_colored_--;
        }

        vertex_colors_.at(idx) = color;
    }

    Color vertex_color(int idx)
    {
        return vertex_colors_.at(idx);
    }

    int n_selected() const
    {
        return n_selected_;
    }

    int n_colored() const
    {
        return n_colored_;
    }

    const std::vector<Color>& vertex_colors() const
    {
        return vertex_colors_;
    }

    const std::vector<bool>& selected_edges() const
    {
        return selected_edges_;
    }

    bool subgraph_connected()
    {
        if (subgraph_connected_) {
            return true;
            // Once the subgraph has been connected all its children
            // are connected.
        }
        else if (subgraph_.is_connected()) {
            subgraph_connected_ = true;
            return true;
        }
        else {
            return false;
        }
    }

    int subgraph_n_edges() const
    {
        return subgraph_.n_edges();
    }

    int total_weight() const
    {
        return total_weight_;
    }

    int potential_weight() const
    {
        return potential_weight_;
    }

    int start_edge_idx() const
    {
        return start_edge_idx_;
    }

    friend std::ostream& operator<<(std::ostream& os, const State& state);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(vertex_colors_);
        archive & BOOST_SERIALIZATION_NVP(selected_edges_);
        archive & BOOST_SERIALIZATION_NVP(subgraph_);
        archive & BOOST_SERIALIZATION_NVP(total_weight_);
        archive & BOOST_SERIALIZATION_NVP(n_selected_);
        archive & BOOST_SERIALIZATION_NVP(n_colored_);
        archive & BOOST_SERIALIZATION_NVP(subgraph_connected_);
        archive & BOOST_SERIALIZATION_NVP(potential_weight_);
        archive & BOOST_SERIALIZATION_NVP(start_edge_idx_);
    }
};

std::ostream& operator<<(std::ostream& os, const State& state)
{
    os << "vertex_colors: " << to_string(state.vertex_colors_) << std::endl
       << "selected_edges: " << to_string(state.selected_edges_) << std::endl
       << "total_weight: " << state.total_weight_ << std::endl
       << "start_edge_idx: " << state.start_edge_idx_ << std::endl;
    return os;
}

#endif //MBPSEQUENTIAL_STATE_H
