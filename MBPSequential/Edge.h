//
// Created by Tomáš Petříček on 18.02.2022.
//

#ifndef MBPSEQUENTIAL_EDGE_H
#define MBPSEQUENTIAL_EDGE_H

struct Edge {
    int vert_from;
    int vert_to;
    int weight;

    Edge(int vertFrom, int vertTo, int weight)
            : vert_from(vertFrom), vert_to(vertTo), weight(weight) {}
};

#endif //MBPSEQUENTIAL_EDGE_H
