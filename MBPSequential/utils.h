//
// Created by Tomáš Petříček on 20.02.2022.
//

#ifndef MBPSEQUENTIAL_UTILS_H
#define MBPSEQUENTIAL_UTILS_H

template<typename T>
std::string to_string(const std::vector<T> &v) {
    std::ostringstream oss;

    if (!v.empty()) {
        std::copy(v.begin(), v.end() - 1,
                  std::ostream_iterator<T>(oss, ","));

        oss << v.back();
    }

    return oss.str();
}

#endif //MBPSEQUENTIAL_UTILS_H
