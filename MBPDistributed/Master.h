//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_MASTER_H
#define MBPDISTRIBUTED_MASTER_H

#include <boost/mpi.hpp>
#include "Finder.h"
#include "Process.h"
#include "State.h"


class Master: public Process<State> {
    void distribute_work()
    {

    }

    void collect_results()
    {

    }

public:
    explicit Master(const boost::mpi::communicator& world)
            :Process(world) { }

    State start() override
    {
        return State(0, 0);
    }

    ~Master() override = default;
};

#endif //MBPDISTRIBUTED_MASTER_H
