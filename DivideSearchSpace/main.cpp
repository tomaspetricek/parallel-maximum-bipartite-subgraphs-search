#include <iostream>
#include <array>
#include <cmath>


struct Group {
    int count;
    int size;
    float ratio;

    Group(int count, int size, float ratio)
            :count(count), size(size), ratio(ratio) { }

    Group() :count(0), size(0), ratio(0.0) { }
};


template<size_t n_vertices>
std::array<Group, n_vertices> create_groups() {
    const int n_children{3};
    std::array<Group, n_vertices> groups;
    int n_states = std::pow(n_children, n_vertices);
    int size, count;
    float ratio;

    for(int i{0}; i < n_vertices; i++) {
        count = std::pow(n_children, i +1);
        size = n_states / count;
        ratio =  static_cast<float>(size) / n_states;
        groups[i] = Group(count, size, ratio);
    }

    return groups;
}

int log(double base, double x) {
    return (int)(std::log(x) / std::log(base));
}

int main() {
    const int n_vertices{15};
    std::array<Group, n_vertices> groups = create_groups<n_vertices>();

    for (const auto& group : groups) {
        std::cout << "Count: " << group.count << ", "
                  << "Size: " << group.size << ",  "
                  << "Ratio: " << group.ratio << std::endl;
    }

    return 0;
}
