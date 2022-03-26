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
#include <boost/mpi/communicator.hpp>

#include "edge_list.h"
#include "state.h"
#include "utils.h"
#include "explorer.h"
#include "process.h"

namespace pdp {
    class finder {
        state best_;
        graph::edge_list graph_;
        long recursion_called_ = 0;
        explorer expl_;
        boost::mpi::communicator world_;
        boost::mpi::request req_;
        state from_best_;
        int from_rank_;
        int to_rank_;

        void select_edge(color from, color to, state curr, explorer* expl = nullptr)
        {
            graph::edge edge = graph_.edge(curr.edge_idx());
            color curr_from = curr.vertex_color(edge.vert_from);
            color curr_to = curr.vertex_color(edge.vert_to);

            if ((curr_from==from || curr_from==colorless) && (curr_to==to || curr_to==colorless)) {
                // change colors
                curr.vertex_color(edge.vert_from, from);
                curr.vertex_color(edge.vert_to, to);

                // select edge
                curr.select_edge(curr.edge_idx(), edge);

                // update index
                curr.edge_idx_++;

                bb_dfs(curr, expl);
            }
        }

        void try_update_best(state candidate) {
            #pragma omp critical
            {
                if (candidate.n_colored()==graph_.n_vertices() && candidate.subgraph_connected()
                        && best_.total_weight()<candidate.total_weight()) {
                    best_ = candidate;
                    world_.isend(to_rank_, process::tag::best, best_);
                }
            }
        }

    public:
        explicit finder(graph::edge_list graph, explorer expl)
                :best_(graph.n_vertices(), graph.n_edges()),
                 graph_(std::move(graph)),
                 expl_(std::move(expl))
        {
            int rank = world_.rank();
            from_rank_ = (rank-1==0) ? world_.size()-1 : rank-1;
            to_rank_ = (rank+1==world_.size()) ? 1 : rank+1;
        }

        // DFS without B&B has complexity: O(3^n), where n is the number of edges.
        // There are 3 options for each edge: without, with 1st coloring order
        // and with 2nd coloring order.
        void bb_dfs(state curr, explorer* expl = nullptr)
        {
            #pragma omp atomic update
            recursion_called_++;

            #pragma omp master
            {
                if (!req_.active())
                    req_ = world_.irecv(from_rank_, process::tag::best, from_best_);

                if (req_.test())
                    try_update_best(from_best_);
            }

            try_update_best(curr);

            while (curr.edge_idx_<graph_.n_edges()) {
                if (expl)
                    if (!expl->keep_exploring(curr))
                        return;

                // check upper bound
                if (curr.total_weight()+(graph_.total_weight()-curr.potential_weight())
                        <best_.total_weight())
                    return;

                // update potential weight
                curr.potential_weight_ += graph_.edge(curr.edge_idx()).weight;

                select_edge(green, red, curr, expl);

                select_edge(red, green, curr, expl);

                // update index
                curr.edge_idx_++;
            }
        }

        std::vector<state> prepare_states(pdp::state init)
        {
            graph_.sort_edges();

            // color start vertex
            init.vertex_color(0, red);

            auto expl = std::make_unique<explorer>(expl_);
            // prepare states
            bb_dfs(init, expl.get());
            return expl->states();
        }

        // Coloring the starting vertex ensures that there is only one way (direction)
        // to color the graph and therefore eliminates half of the possible solutions.
        state find(const pdp::state& init)
        {
            std::vector<state> states = prepare_states(init);

            if (states.size()==0)
                return best_;

            // find best state
            #pragma omp parallel for
            for (int i = 0; i<states.size(); i++) {
                bb_dfs(states[i]);
            }

            return best_;
        }

        const state& best() const
        {
            return best_;
        }
        void best(const state& best)
        {
            best_ = best;
        }

        long recursion_called() const
        {
            return recursion_called_;
        }
    };
}

#endif //MBPSEQUENTIAL_FINDER_H
