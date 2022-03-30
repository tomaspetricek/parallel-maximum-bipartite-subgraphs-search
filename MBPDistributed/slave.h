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
        state prev_best_;
        int prev_rank_;
        int next_rank_;
        volatile bool searching_ = true;
        boost::mpi::request req_recv_;
        boost::mpi::request req_send_;
        long best_updated_count_ = 0;

    public:
        explicit slave(boost::mpi::communicator world)
                :world_(std::move(world))
        {
            int rank = world_.rank();
            prev_rank_ = (rank-1==0) ? world_.size()-1 : rank-1;
            next_rank_ = (rank+1==world_.size()) ? 1 : rank+1;
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

                omp_set_max_active_levels(2);
                #pragma omp parallel num_threads(2)
                {
                    if (omp_get_thread_num()==0) {
//                        req_recv_ = world_.irecv(prev_rank_, process::tag::best, prev_best_);
//                        req_send_ = world_.isend(next_rank_, process::tag::best, finder.best());
//
//                        while (searching_) {
//                            if (req_recv_.test()) {
//                                finder.try_update_best(prev_best_);
//                                req_recv_ = world_.irecv(prev_rank_, process::tag::best, prev_best_);
//                            }
//
//                            if (req_send_.test() && best_updated_count_<finder.best_updated_count()) {
//                                best_updated_count_ = finder.best_updated_count();
//                                req_send_ = world_.isend(next_rank_, process::tag::best, finder.best());
//                            }
//                        }
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
