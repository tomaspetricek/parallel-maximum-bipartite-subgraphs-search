//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_EDGELISTGRAPH_H
#define MBPSEQUENTIAL_EDGELISTGRAPH_H

#include <utility>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "edge.h"

namespace pdp::graph {
    class edge_list {
        int n_vertices_ = 0;
        int n_edges_ = 0;
        std::vector<pdp::graph::edge> edges_;
        int total_weight_ = 0;

    public:
        explicit edge_list(const int n_vertices)
                :n_vertices_(n_vertices), n_edges_(0), total_weight_(0) { }

        edge_list() = default;

        void add_edge(graph::edge edge)
        {
            if (edge.vert_from>=n_vertices_ || edge.vert_to>=n_vertices_)
                throw std::out_of_range("Vertex index out of range");

            edges_.emplace_back(edge);
            n_edges_++;
            total_weight_ += edge.weight;
        }

        void sort_edges()
        {
            // sort edges in descending order
            std::sort(edges_.begin(), edges_.end(), std::greater<>());
        }

        int n_vertices() const
        {
            return n_vertices_;
        }

        int n_edges() const
        {
            return edges_.size();
        }

        pdp::graph::edge edge(unsigned int idx) const
        {
            if (idx>=edges_.size())
                throw std::out_of_range("Edge index out of range");

            return edges_.at(idx);
        }

        int total_weight() const
        {
            return total_weight_;
        }

        const std::vector<pdp::graph::edge>& edges() const
        {
            return edges_;
        }

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(n_vertices_);
            archive & BOOST_SERIALIZATION_NVP(n_edges_);
            archive & BOOST_SERIALIZATION_NVP(edges_);
            archive & BOOST_SERIALIZATION_NVP(total_weight_);
        }
    };
}

#endif //MBPSEQUENTIAL_EDGELISTGRAPH_H
