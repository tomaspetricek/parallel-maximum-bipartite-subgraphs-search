//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_PROCESS_H
#define MBPDISTRIBUTED_PROCESS_H

#include <boost/mpi/communicator.hpp>


template<typename result>
class Process {
protected:
    static const int work_tag = 0;
    static const int master_rank = 0;
    boost::mpi::communicator world_;

protected:
    explicit Process(boost::mpi::communicator  world)
            :world_(std::move(world)) { }

public:
    virtual result start() = 0;

    virtual ~Process() = default;
};

#endif //MBPDISTRIBUTED_PROCESS_H
