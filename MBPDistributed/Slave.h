//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_SLAVE_H
#define MBPDISTRIBUTED_SLAVE_H

#include <boost/mpi.hpp>
#include "Finder.h"
#include "Process.h"

class Slave : public Process<void> {
public:
    explicit Slave(const boost::mpi::communicator& world)
            :Process(world) { }

    void start() override
    {

    }

    ~Slave() override = default;
};
#endif //MBPDISTRIBUTED_SLAVE_H
