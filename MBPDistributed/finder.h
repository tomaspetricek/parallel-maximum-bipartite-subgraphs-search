//
// Created by Tomáš Petříček on 17.02.2022.
//

#ifndef MBPSEQUENTIAL_FINDER_H
#define MBPSEQUENTIAL_FINDER_H

#include <utility>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>
#include <omp.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "edge_graph.h"
#include "state.h"
#include "utils.h"
#include "explorer.h"

namespace pdp {
    class finder {
        state init_;
        state best_;
        edge_graph graph_;
        long recursion_called_ = 0;
        std::shared_ptr<explorer> expl_;

    public:
        explicit finder(edge_graph graph, std::shared_ptr<explorer> expl)
                :init_(graph.n_vertices(), graph.n_edges()),
                 best_(graph.n_vertices(), graph.n_edges()),
                 graph_(std::move(graph)),
                 expl_(std::move(expl)) { }

        finder(state initial, state best, edge_graph graph, std::shared_ptr<explorer> expl)
                :init_(std::move(initial)),
                 best_(std::move(best)),
                 graph_(std::move(graph)),
                 expl_(std::move(expl)) { }

        finder() = default;

        // DFS without B&B has complexity: O(3^n), where n is the number of edges.
        // There are 3 options for each edge: without, with 1st coloring order
        // and with 2nd coloring order.
        void bb_dfs(state curr, explorer* expl = nullptr)
        {
            #pragma omp atomic update
            recursion_called_++;

            #pragma omp critical
            if (curr.n_colored()==graph_.n_vertices() && curr.subgraph_connected()
                    && best_.total_weight()<curr.total_weight())
                best_ = curr;

            for (int edge_idx = curr.start_edge_idx(); edge_idx<graph_.n_edges(); edge_idx++) {
                if (expl)
                    if (!expl->keep_exploring(curr))
                        return;

                // check upper bound
                if (curr.total_weight()+(graph_.total_weight()-curr.potential_weight())
                        <best_.total_weight())
                    return;

                // update curr_state
                curr.potential_weight_ += graph_.edge(edge_idx).weight;
                curr.start_edge_idx_++;

                select_edge(green, red, curr, edge_idx, expl);

                select_edge(red, green, curr, edge_idx, expl);
            }
        }

        void select_edge(color from, color to, state curr, int edge_idx, explorer* expl = nullptr)
        {
            edge edge = graph_.edge(edge_idx);

            color curr_from = curr.vertex_color(edge.vert_from);
            color curr_to = curr.vertex_color(edge.vert_to);

            if ((curr_from==from || curr_from==colorless) && (curr_to==to || curr_to==colorless)) {
                // change colors
                curr.vertex_color(edge.vert_from, from);
                curr.vertex_color(edge.vert_to, to);

                // select edge
                curr.select_edge(edge_idx, edge);

                bb_dfs(curr, expl);
            }
        }

        std::vector<state> prepare_states()
        {
            graph_.sort_edges();

            // color start vertex
            init_.vertex_color(0, red);

            // prepare states
            bb_dfs(init_, expl_.get());
            return expl_->states();
        }

        // Coloring the starting vertex ensures that there is only one way (direction)
        // to color the graph and therefore eliminates half of the possible solutions.
        state find()
        {
            std::vector<state> states = prepare_states();

            assert(states.size()>0);

            // find best state
            #pragma omp parallel for
            for (int i = 0; i<states.size(); i++) {
                bb_dfs(states[i]);
            }

            return best_;
        }

        long recursion_called() const
        {
            return recursion_called_;
        }

        const state& best() const
        {
            return best_;
        }

        const state& init() const
        {
            return init_;
        }

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(init_);
            archive & BOOST_SERIALIZATION_NVP(best_);
            archive & BOOST_SERIALIZATION_NVP(graph_);
            archive & BOOST_SERIALIZATION_NVP(recursion_called_);
            archive & BOOST_SERIALIZATION_NVP(expl_);
        }
    };
}

#endif //MBPSEQUENTIAL_FINDER_H
