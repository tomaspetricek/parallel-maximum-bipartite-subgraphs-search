//
// Created by Tomáš Petříček on 26.03.2022.
//

#ifndef MBPDISTRIBUTED_CONFIG_H
#define MBPDISTRIBUTED_CONFIG_H

#include <utility>

#include <boost/archive/binary_oarchive.hpp>

#include "state.h"

namespace pdp {
    struct config {
        state init;
        state best;

        explicit config(state  init, state  best)
                :init(std::move(init)), best(std::move(best)) { }

        config() = default;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(init);
            archive & BOOST_SERIALIZATION_NVP(best);
        }
    };
}
#endif //MBPDISTRIBUTED_CONFIG_H
