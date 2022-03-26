//
// Created by Tomáš Petříček on 20.03.2022.
//

#ifndef MBPDISTRIBUTED_PROCESS_H
#define MBPDISTRIBUTED_PROCESS_H

namespace pdp::process {
    namespace tag {
        static const int setting = 0;
        static const int config = 1;
        static const int done = 2;
        static const int stop = 3;
        static const int best = 4;
    }

    namespace rank {
        static const int master = 0;
    }
}

#endif //MBPDISTRIBUTED_PROCESS_H
