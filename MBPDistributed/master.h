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

namespace pdp::process {
    class master {
        boost::mpi::communicator world_;
        pdp::graph::edge_list graph_;
        pdp::state best_;

    public:
        master(boost::mpi::communicator world, pdp::graph::edge_list graph)
                :world_(std::move(world)), graph_(std::move(graph)) { }

        state start()
        {
            auto states = prepare_states();
            assert(states.size()>=world_.size()-1);
            manage_slaves(states);
            return best_;
        }

    private:
        std::vector<state> prepare_states()
        {
            int max_depth{3};
            std::shared_ptr<pdp::explorer> expl = std::make_shared<pdp::explorer>(graph_.n_edges(), max_depth);
            finder finder(graph_, std::move(expl));

            std::vector<state> states = finder.prepare_states();
            best_ = finder.best();
            return states;
        }

        int collect_results()
        {
            state local_best;
            boost::mpi::status status = world_.recv(boost::mpi::any_source, done_tag, local_best);

            if (local_best.total_weight()>best_.total_weight())
                best_ = local_best;

            return status.source();
        }

        void manage_slaves(const std::vector<pdp::state>& states)
        {
            int source;
            std::shared_ptr<pdp::explorer> expl = std::make_shared<pdp::explorer>(graph_.n_edges(), 4);

            for (int i{0}; i<states.size()+world_.size(); i++) {
                // seed slaves
                if (i<world_.size()-1) {
                    finder finder(states[i], best_, graph_, expl);
                    world_.send(i+1, work_tag, finder);
                }
                // give work
                else if (i<states.size()) {
                    source = collect_results();
                    finder finder(states[i], best_, graph_, expl);
                    world_.send(source, work_tag, finder);
                }
                // stop slaves
                else if (i>states.size()) {
                    source = collect_results();
                    world_.send(source, stop_tag, finder());
                }
            }
        }
    };
}

#endif //MBPDISTRIBUTED_MASTER_H
