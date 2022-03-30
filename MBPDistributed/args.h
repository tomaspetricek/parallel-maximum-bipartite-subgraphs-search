//
// Created by Tomáš Petříček on 30.03.2022.
//

#ifndef MBPDISTRIBUTED_ARGS_H
#define MBPDISTRIBUTED_ARGS_H

#include <map>
#include <string>

namespace pdp {
    class args {
        std::map<std::string, std::string> args_;

    public:
        static pdp::args parse(int argc, char* argv[]) {
            pdp::args args;
            std::string val;
            std::string opt;

            for (int i{1}; i<argc; i++) {
                opt = argv[i];
                std::erase(opt, '-');
                val = argv[++i];
                args.set(opt, val);
            }

            return args;
        }

        std::string get(const std::string& opt) {
            auto it = args_.find(opt);

            if (it == args_.end())
                throw std::runtime_error("Argument not found: " + opt);

            return it->second;
        }

        void set(const std::string& arg, const std::string& val) {
            args_[arg] = val;
        }
    };
}

#endif //MBPDISTRIBUTED_ARGS_H
