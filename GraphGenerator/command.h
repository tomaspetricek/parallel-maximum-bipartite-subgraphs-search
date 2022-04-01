//
// Created by Tomáš Petříček on 31.03.2022.
//

#ifndef GRAPHGENERATOR_COMMAND_H
#define GRAPHGENERATOR_COMMAND_H

#include <map>
#include <sstream>

class generator {
    std::map<char, std::string> flags_;

public:
    enum graph_type {
        regular,
        ad
    };

    void graph_type(graph_type type)
    {
        if (type==ad) {
            flags_['t'] = "AD";
        }
    }

    void n_vertices(int n_vertices)
    {
        flags_['n'] = std::to_string(n_vertices);
    }

    void degree_edges(int degree_edges)
    {
        flags_['k'] = std::to_string(degree_edges);
    }

    void output_filename(const std::string& output_filename)
    {
        flags_['o'] = output_filename;
    }

    void connected_required()
    {
        flags_['c'] = std::string();
    }

    void weights_interval(int min, int max)
    {
        std::string interval = std::to_string(min)+","+std::to_string(max);
        flags_['w'] = interval;
    }

    std::string str()
    {
        std::stringstream ss;
        ss << "./generator";

        for (const auto& flag : flags_) {
            ss << " -" << flag.first;

            if (!flag.second.empty()) {
                if (flag.first!='w') {
                    ss << " ";
                }
                ss << flag.second;
            }
        }

        return ss.str();
    }
};

#endif //GRAPHGENERATOR_COMMAND_H
