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
            int max_depth{1};

            // prepare states
            std::shared_ptr<pdp::explorer> expl = std::make_shared<pdp::explorer>(graph_.n_edges(), max_depth);
            finder finder(graph_, std::move(expl));
            std::vector<state> states = finder.prepare_states();
            best_ = finder.best();

            distribute_work(states);
            collect_results();
            return best_;
        }

    private:
        void distribute_work(const std::vector<pdp::state>& states)
        {
            int rank{0};

            assert(states.size()==world_.size()-1);

            for (std::size_t i{0}; i<states.size(); i++) {
                std::shared_ptr<pdp::explorer> expl = std::make_shared<pdp::explorer>(graph_.n_edges(), 4);
                finder finder(states[i], best_, graph_, expl);
                rank = static_cast<int>(i+1);
                world_.send(rank, work_tag, finder);
            }
        }

        void collect_results()
        {
            state local_best;
            for (int done{0}; done<world_.size()-1; done++) {
                world_.recv(boost::mpi::any_source, done_tag, local_best);
                std::cout << "Local best: " << std::endl << local_best << std::endl;

                if (local_best.total_weight()>best_.total_weight())
                    best_ = local_best;
            }
        }
    };
}

#endif //MBPDISTRIBUTED_MASTER_H
