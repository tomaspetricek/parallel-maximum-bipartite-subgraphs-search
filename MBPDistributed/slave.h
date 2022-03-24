//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_SLAVE_H
#define MBPDISTRIBUTED_SLAVE_H

#include <utility>

#include <boost/mpi.hpp>
#include <boost/mpi/communicator.hpp>

#include "finder.h"
#include "process.h"

namespace pdp::process {
    class slave {
        boost::mpi::communicator world_;

    public:
        explicit slave(boost::mpi::communicator world)
                :world_(std::move(world)) { }

        void start()
        {
            boost::mpi::status status;
            state best;

            while (true) {
                finder finder;
                status = world_.recv(master_rank, boost::mpi::any_tag, finder);

                if (status.tag()==stop_tag)
                    return;

                best = finder.find();
                world_.send(master_rank, done_tag, best);
            }
        }
    };
}
#endif //MBPDISTRIBUTED_SLAVE_H
