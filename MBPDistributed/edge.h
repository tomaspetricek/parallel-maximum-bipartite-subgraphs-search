//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_EDGE_H
#define MBPSEQUENTIAL_EDGE_H

#include <ostream>
#include <boost/archive/binary_oarchive.hpp>

namespace pdp {
    struct edge {
        int vert_from;
        int vert_to;
        int weight;

        edge(int vert_from, int vert_to, int weight)
                :vert_from(vert_from), vert_to(vert_to), weight(weight) { }

        edge() = default;

        bool operator<(const edge& rhs) const
        {
            return weight<rhs.weight;
        }

        bool operator>(const edge& rhs) const
        {
            return rhs<*this;
        }

        bool operator<=(const edge& rhs) const
        {
            return !(rhs<*this);
        }

        bool operator>=(const edge& rhs) const
        {
            return !(*this<rhs);
        }

        friend std::ostream& operator<<(std::ostream& os, const edge& edge)
        {
            os << "vert_from: " << edge.vert_from << " vert_to: " << edge.vert_to << " weight: " << edge.weight;
            return os;
        }

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(vert_from);
            archive & BOOST_SERIALIZATION_NVP(vert_to);
            archive & BOOST_SERIALIZATION_NVP(weight);
        }
    };
}

#endif //MBPSEQUENTIAL_EDGE_H
