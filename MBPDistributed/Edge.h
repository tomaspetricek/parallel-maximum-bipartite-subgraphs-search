//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_EDGE_H
#define MBPSEQUENTIAL_EDGE_H

#include <ostream>
#include <boost/archive/binary_oarchive.hpp>


struct Edge {
    int vert_from;
    int vert_to;
    int weight;

    Edge(int vert_from, int vert_to, int weight)
            : vert_from(vert_from), vert_to(vert_to), weight(weight) {}

    Edge() = default;

    bool operator<(const Edge &rhs) const {
        return weight < rhs.weight;
    }

    bool operator>(const Edge &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Edge &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Edge &rhs) const {
        return !(*this < rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const Edge &edge) {
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

#endif //MBPSEQUENTIAL_EDGE_H
