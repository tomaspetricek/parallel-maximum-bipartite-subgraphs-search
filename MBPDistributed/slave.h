//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_SLAVE_H
#define MBPDISTRIBUTED_SLAVE_H

#include <boost/mpi.hpp>
#include <utility>
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
            finder finder;
            world_.recv(master_rank, work_tag, finder);

            state best = finder.find();
            world_.send(master_rank, done_tag, best);
        }
    };
}
#endif //MBPDISTRIBUTED_SLAVE_H
