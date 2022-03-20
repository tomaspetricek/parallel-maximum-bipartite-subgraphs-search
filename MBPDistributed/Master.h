//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_MASTER_H
#define MBPDISTRIBUTED_MASTER_H

#include <utility>

#include <boost/mpi.hpp>
#include <boost/mpi/communicator.hpp>

#include "Finder.h"
#include "Process.h"
#include "State.h"
#include "AdjacencyGraph.h"

class Master : public Process<State> {
    EdgeGraph graph_;
    State best_;

public:
    Master(const boost::mpi::communicator& world, EdgeGraph graph)
            :Process(world), graph_(std::move(graph)) { }

    State start() override
    {
        int max_depth{1};

        // prepare states
        std::shared_ptr<Explorer> expl = std::make_shared<Explorer>(graph_.n_edges(), max_depth);
        Finder finder(graph_, std::move(expl));
        std::vector<State> states = finder.prepare_states();
        best_ = finder.best();

        distribute_work(states);
        collect_results();
        return best_;
    }

    ~Master() override = default;

private:
    void distribute_work(const std::vector<State>& states)
    {
        int rank{0};

        assert(states.size()==world_.size()-1);

        for (std::size_t i{0}; i<states.size(); i++) {
            std::shared_ptr<Explorer> expl = std::make_shared<Explorer>(graph_.n_edges(), 2);
            Finder finder(states[i], best_, graph_, expl);
            rank = static_cast<int>(i+1);
            world_.send(rank, work_tag, finder);
        }
    }

    void collect_results()
    {
        State local_best;
        for (int done{0}; done<world_.size()-1; done++) {
            world_.recv(boost::mpi::any_source, work_tag, local_best);
            std::cout << "Local best: " << std::endl << local_best << std::endl;

            if (local_best.total_weight()>best_.total_weight())
                best_ = local_best;
        }
    }
};

#endif //MBPDISTRIBUTED_MASTER_H
