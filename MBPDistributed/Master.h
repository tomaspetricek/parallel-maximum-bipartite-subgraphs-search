//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_MASTER_H
#define MBPDISTRIBUTED_MASTER_H

#include <boost/mpi.hpp>
#include <utility>
#include "Finder.h"
#include "Process.h"
#include "State.h"
#include "AdjacencyGraph.h"


class Master: public Process<State> {
    EdgeGraph graph;

public:
    Master(const boost::mpi::communicator& world, EdgeGraph graph)
            :Process(world), graph(std::move(graph)) { }

    State start() override
    {
        std::unique_ptr<Explorer> expl = std::make_unique<Explorer>(graph.n_edges(), 0);
        Finder finder(graph, std::move(expl));

        world_.send(1, work_tag, finder);

        State best;
        world_.recv(1, work_tag, best);

        return best;
    }

    ~Master() override = default;

private:
    void distribute_work()
    {

    }

    void collect_results()
    {

    }
};

#endif //MBPDISTRIBUTED_MASTER_H
