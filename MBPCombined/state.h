//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_STATE_H
#define MBPSEQUENTIAL_STATE_H

#include <ostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "color.h"
#include "utils.h"
#include "adjacency_list.h"

namespace pdp::finder {
    class state {
        std::vector<color> vertex_colors_;
        std::vector<bool> selected_edges_;
        graph::adjacency_list subgraph_;
        int total_weight_ = 0;
        int n_selected_ = 0;
        int n_colored_ = 0;
        bool subgraph_connected_ = false;
        int potential_weight_ = 0;
        unsigned int edge_idx_ = 0;
        friend class data_parallel;
        friend class task_parallel;
        friend class sequential;

    public:
        explicit state(int n_vertices, int n_edges)
                :vertex_colors_(std::vector<color>(n_vertices, colorless)),
                 selected_edges_(std::vector<bool>(n_edges, false)),
                 subgraph_(n_vertices) { }

        state() = default;

        void select_edge(unsigned int idx, const graph::edge& edge)
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

        void vertex_color(int idx, color color)
        {
            if (vertex_colors_.at(idx)==colorless && color!=colorless) {
                n_colored_++;
            }
            else if (vertex_colors_.at(idx)!=colorless && color==colorless) {
                n_colored_--;
            }

            vertex_colors_.at(idx) = color;
        }

        color vertex_color(int idx)
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

        const std::vector<color>& vertex_colors() const
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

        unsigned int edge_idx() const
        {
            return edge_idx_;
        }

        friend std::ostream& operator<<(std::ostream& os, const state& state);

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
            archive & BOOST_SERIALIZATION_NVP(edge_idx_);
        }
    };

    std::ostream& operator<<(std::ostream& os, const state& state)
    {
        os << std::endl
           << "vertex_colors: " << to_string(state.vertex_colors_) << std::endl
           << "selected_edges: " << to_string(state.selected_edges_) << std::endl
           << "total_weight: " << state.total_weight_ << std::endl
           << "edge_idx: " << state.edge_idx_;
        return os;
    }
}

#endif //MBPSEQUENTIAL_STATE_H
