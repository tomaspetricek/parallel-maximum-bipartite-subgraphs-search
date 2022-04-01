//
// Created by Tomáš Petříček on 26.03.2022.
//

#ifndef MBPDISTRIBUTED_SETTING_H
#define MBPDISTRIBUTED_SETTING_H

#include <utility>

#include <boost/archive/binary_oarchive.hpp>

#include "edge_list.h"
#include "explorer.h"

namespace pdp {
    struct setting {
        graph::edge_list graph;
        finder::explorer explorer;

        explicit setting(graph::edge_list graph, finder::explorer explorer)
                :graph(std::move(graph)), explorer(std::move(explorer)) { }

        setting() = default;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(graph);
            archive & BOOST_SERIALIZATION_NVP(explorer);
        }
    };
}
#endif //MBPDISTRIBUTED_SETTING_H
