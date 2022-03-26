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
#include "setting.h"

namespace pdp::process {
    class slave {
        boost::mpi::communicator world_;
        state from_best_;
        int from_rank_;
        int to_rank_;
        volatile bool searching_ = true;
        boost::mpi::request req_recv_;
        boost::mpi::request req_send_;
        bool send;
        bool received;

    public:
        explicit slave(boost::mpi::communicator world)
                :world_(std::move(world))
        {
            int rank = world_.rank();
            from_rank_ = (rank-1==0) ? world_.size()-1 : rank-1;
            to_rank_ = (rank+1==world_.size()) ? 1 : rank+1;
        }

        void start()
        {
            boost::mpi::status status;
            state best;
            pdp::config config;

            pdp::setting setting;
            world_.recv(rank::master, tag::setting, setting);;

            finder finder{setting.graph, setting.explorer};

            while (true) {
                status = world_.recv(rank::master, boost::mpi::any_tag, config);

                if (status.tag()==tag::stop) {
                    return;
                }

                searching_ = true;
                send = false;
                received = false;
                long best_updated_count = finder.best_updated_count();

                omp_set_max_active_levels(2);
                #pragma omp parallel num_threads(2)
                {
                    if (omp_get_thread_num()==0) {
                        while (searching_) {
                            if (!received) {
                                req_send_ = world_.irecv(from_rank_, process::tag::best, from_best_);
                                received = true;
                            } else if (req_send_.test()) {
                                finder.try_update_best(from_best_);
                                received = false;
                            }

                            if (!send && best_updated_count < finder.best_updated_count()) {
                                best_updated_count = finder.best_updated_count();
                                req_send_ = world_.isend(to_rank_, process::tag::best, finder.best());
                                send = true;
                            } else if (req_send_.test()){
                                send = false;
                            }
                        }
                    }
                    else {
                        finder.best(config.best);
                        best = finder.find(config.init);
                        searching_ = false;
                    }
                }
                world_.send(rank::master, tag::done, best);
            }
        }
    };
}
#endif //MBPDISTRIBUTED_SLAVE_H
