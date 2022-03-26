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
            pdp::config config;

            finder finder;
            world_.recv(rank::master, tag::finder, finder);

            while (true) {
                status = world_.recv(rank::master, boost::mpi::any_tag, config);

                if (status.tag()==tag::stop) {
                    return;
                }

                finder.best(config.best);
                best = finder.find(config.init);
                world_.send(rank::master, tag::done, best);
            }
        }
    };
}
#endif //MBPDISTRIBUTED_SLAVE_H
