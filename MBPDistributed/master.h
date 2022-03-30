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
        graph::edge_list graph_;
        state best_;
        explorer slave_explorer_;
        finder finder_;

    public:
        explicit master(boost::mpi::communicator world, graph::edge_list graph, explorer master_explorer,
                explorer slave_explorer)
                :world_(std::move(world)), graph_(std::move(graph)),
                 slave_explorer_(std::move(slave_explorer)),
                 finder_(graph_, std::move(master_explorer)) { }

        state start()
        {
            auto init_states = prepare_states();

            if (init_states.size()<world_.size()-1)
                throw std::runtime_error("Not enough work for all slaves");

            manage_slaves(init_states);
            return best_;
        }

    private:
        std::vector<state> prepare_states()
        {
            state root = state(graph_.n_vertices(), graph_.n_edges());
            std::vector<state> states = finder_.prepare_states(root);
            best_ = finder_.best();
            return states;
        }

        void manage_slaves(const std::vector<pdp::state>& init_states)
        {
            pdp::setting setting(graph_, slave_explorer_);
            state local_best;
            boost::mpi::status status;

            for(int i{0}; i<init_states.size()+world_.size(); i++) {
                // start working
                if (i<world_.size()-1) {
                    world_.send(i+1, tag::setting, setting);
                    world_.send(i+1, tag::config, pdp::config(init_states[i], best_));
                }
                // keep working
                else if (i<init_states.size()) {
                    status = world_.recv(boost::mpi::any_source, tag::done, local_best);

                    // try update best
                    if (local_best.total_weight()>best_.total_weight())
                        best_ = local_best;

                    // send config
                    world_.send(status.source(), tag::config, pdp::config(init_states[i], best_));
                }
                // stop working
                else if (i>init_states.size()) {
                    status = world_.recv(boost::mpi::any_source, tag::done, local_best);

                    // try update best
                    if (local_best.total_weight()>best_.total_weight())
                        best_ = local_best;

                    // stop slave
                    world_.send(status.source(), tag::stop, pdp::config());
                }
            }
        }
    };
}

#endif //MBPDISTRIBUTED_MASTER_H
