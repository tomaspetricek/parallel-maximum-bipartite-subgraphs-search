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

namespace pdp::finder {
    class data_parallel {
        state best_;
        graph::edge_list graph_;
        long recursion_called_ = 0;
        explorer explorer_;

        void select_edge(color from, color to, state curr, explorer* explorer = nullptr)
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

                bb_dfs(curr, explorer);
            }
        }

    public:
        explicit data_parallel(graph::edge_list graph, explorer explorer)
                :best_(graph.n_vertices(), graph.n_edges()),
                 graph_(std::move(graph)),
                 explorer_(std::move(explorer))
        {
            // sort edges
            graph_.sort_edges();
        }

        void try_update_best(state candidate)
        {
#pragma omp critical
            {
                if (candidate.n_colored()==graph_.n_vertices() && candidate.subgraph_connected()
                        && best_.total_weight()<candidate.total_weight()) {
                    best_ = candidate;
                }
            }
        }

        // Checks upper bound.
        bool can_be_better(const state& best, const state& other)
        {
            return (other.total_weight()+(graph_.total_weight()-other.potential_weight()))
                    >best.total_weight();
        }

        // DFS without B&B has complexity: O(3^n), where n is the number of edges.
        // There are 3 options for each edge: without, with 1st coloring order
        // and with 2nd coloring order.
        void bb_dfs(state curr, explorer* explorer = nullptr)
        {
#pragma omp atomic update
            recursion_called_++;

            try_update_best(curr);

            while (curr.edge_idx_<graph_.n_edges()) {
                if (explorer)
                    if (!explorer->keep_exploring(curr))
                        return;

                // check upper bound
                if (!can_be_better(best_, curr))
                    return;

                // update potential weight
                curr.potential_weight_ += graph_.edge(curr.edge_idx()).weight;

                select_edge(green, red, curr, explorer);

                select_edge(red, green, curr, explorer);

                // update index
                curr.edge_idx_++;
            }
        }

        std::vector<state> prepare_states(state init)
        {
            // color start vertex
            init.vertex_color(0, red);

            auto expl = std::make_unique<explorer>(explorer_);
            // prepare states
            bb_dfs(init, expl.get());
            return expl->states();
        }

        // Coloring the starting vertex ensures that there is only one way (direction)
        // to color the graph and therefore eliminates half of the possible solutions.
        state find(const state& init)
        {
            if (!can_be_better(best_, init))
                return best_;

            std::vector<state> states = prepare_states(init);

            assert((states.size()==0, "No states to search"));

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

    class task_parallel {
        state best_;
        graph::edge_list graph_;
        long recursion_called_ = 0;
        unsigned int n_sequential_;

        static float validate_ratio(float ratio)
        {
            if (ratio>=0.0 && ratio<=1.0)
                return ratio;
            else
                throw std::out_of_range("Ratio out of range <0.0, 1.0>");
        }

        void select_edge(color from, color to, state curr)
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

                if (graph_.n_edges()-curr.edge_idx()-1>n_sequential_) {
#pragma omp task
                    bb_dfs(curr);
                }
                else {
                    bb_dfs(curr);
                }
            }
        }

    public:
        explicit task_parallel(graph::edge_list graph, float sequential_ratio)
                :best_(graph.n_vertices(), graph.n_edges()),
                 graph_(std::move(graph)),
                 n_sequential_(static_cast<int>(static_cast<float>(graph.n_edges())*validate_ratio(sequential_ratio)))
        {
            // sort edges
            graph_.sort_edges();
        }

        void try_update_best(state candidate)
        {
#pragma omp critical
            {
                if (candidate.n_colored()==graph_.n_vertices() && candidate.subgraph_connected()
                        && best_.total_weight()<candidate.total_weight()) {
                    best_ = candidate;
                }
            }
        }

        // Checks upper bound.
        bool can_be_better(const state& best, const state& other)
        {
            return (other.total_weight()+(graph_.total_weight()-other.potential_weight()))
                    >best.total_weight();
        }

        // DFS without B&B has complexity: O(3^n), where n is the number of edges.
        // There are 3 options for each edge: without, with 1st coloring order
        // and with 2nd coloring order.
        void bb_dfs(state curr)
        {
#pragma omp atomic update
            recursion_called_++;

            try_update_best(curr);

            while (curr.edge_idx_<graph_.n_edges()) {
                // check upper bound
                if (!can_be_better(best_, curr))
                    return;

                // update potential weight
                curr.potential_weight_ += graph_.edge(curr.edge_idx()).weight;

                select_edge(green, red, curr);

                select_edge(red, green, curr);

#pragma omp taskwait

                // update index
                curr.edge_idx_++;
            }
        }

        // Coloring the starting vertex ensures that there is only one way (direction)
        // to color the graph and therefore eliminates half of the possible solutions.
        state find(const state& init)
        {
            // color start vertex
            best_.vertex_color(0, red);

            // find best state
#pragma omp parallel
#pragma omp master
            bb_dfs(init);

            return best_;
        }

        long recursion_called() const
        {
            return recursion_called_;
        }
    };

    class sequential {
        state best_;
        graph::edge_list graph_;
        long recursion_called_ = 0;

        void select_edge(color from, color to, state curr)
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

                bb_dfs(curr);
            }
        }

    public:
        explicit sequential(graph::edge_list graph)
                :best_(graph.n_vertices(), graph.n_edges()),
                 graph_(std::move(graph))
        {
            // sort edges
            graph_.sort_edges();
        }

        void try_update_best(state candidate)
        {

            if (candidate.n_colored()==graph_.n_vertices() && candidate.subgraph_connected()
                    && best_.total_weight()<candidate.total_weight()) {
                best_ = candidate;
            }
        }

        // Checks upper bound.
        bool can_be_better(const state& best, const state& other)
        {
            return (other.total_weight()+(graph_.total_weight()-other.potential_weight()))
                    >best.total_weight();
        }

        // DFS without B&B has complexity: O(3^n), where n is the number of edges.
        // There are 3 options for each edge: without, with 1st coloring order
        // and with 2nd coloring order.
        void bb_dfs(state curr)
        {
            recursion_called_++;

            try_update_best(curr);

            while (curr.edge_idx_<graph_.n_edges()) {
                // check upper bound
                if (!can_be_better(best_, curr))
                    return;

                // update potential weight
                curr.potential_weight_ += graph_.edge(curr.edge_idx()).weight;

                select_edge(green, red, curr);

                select_edge(red, green, curr);

                // update index
                curr.edge_idx_++;
            }
        }

        // Coloring the starting vertex ensures that there is only one way (direction)
        // to color the graph and therefore eliminates half of the possible solutions.
        state find(const state& init)
        {
            // color start vertex
            best_.vertex_color(0, red);

            // find best state
            bb_dfs(init);

            return best_;
        }

        long recursion_called() const
        {
            return recursion_called_;
        }
    };
}

#endif //MBPSEQUENTIAL_FINDER_H
