//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_MASTER_H
#define MBPDISTRIBUTED_MASTER_H

#include <utility>

#include <boost/mpi.hpp>
#include <boost/mpi/communicator.hpp>

#include "finder.h"
#include "process.h"
#include "state.h"
#include "adjacency_list.h"
#include "config.h"
#include "setting.h"

namespace pdp::process {
    class master {
        boost::mpi::communicator world_;
        pdp::graph::edge_list graph_;
        pdp::state best_;
        pdp::explorer master_explorer_;
        pdp::explorer slave_explorer_;

    public:
        master(boost::mpi::communicator world, graph::edge_list graph, explorer master_explorer,
                explorer slave_explorer)
                :world_(std::move(world)), graph_(std::move(graph)), master_explorer_(std::move(master_explorer)),
                 slave_explorer_(std::move(slave_explorer)) { }

        state start()
        {
            auto states = prepare_states();

            if (states.size()<world_.size()-1)
                throw std::runtime_error("Not enough initial states for slave process.");

            manage_slaves(states);
            return best_;
        }

    private:
        std::vector<state> prepare_states()
        {
            finder finder(graph_, master_explorer_);
            std::vector<state> states = finder.prepare_states(state(graph_.n_vertices(), graph_.n_edges()));
            best_ = finder.best();
            return states;
        }

        int collect_results()
        {
            state local_best;
            boost::mpi::status status = world_.recv(boost::mpi::any_source, tag::done, local_best);

            // update best state
            if (local_best.total_weight()>best_.total_weight())
                best_ = local_best;

            return status.source();
        }

        void manage_slaves(const std::vector<pdp::state>& states)
        {
            int source;
            pdp::setting setting(graph_, slave_explorer_);

            for (int i{0}; i<states.size()+world_.size(); i++) {
                // start working
                if (i<world_.size()-1) {
                    world_.send(i+1, tag::setting, setting);
                    world_.send(i+1, tag::config, pdp::config(states[i], best_));
                }
                // keep working
                else if (i<states.size()) {
                    source = collect_results();
                    world_.send(source, tag::config, pdp::config(states[i], best_));
                }
                // stop working
                else if (i>states.size()) {
                    source = collect_results();
                    world_.send(source, tag::stop, pdp::config());
                }
            }
        }
    };
}

#endif //MBPDISTRIBUTED_MASTER_H
