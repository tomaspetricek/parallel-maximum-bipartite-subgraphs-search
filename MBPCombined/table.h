//
// Created by Tomáš Petříček on 21.12.2021.
//

#ifndef BOOLEANSATISFIABILITYPROBLEM_TABLE_H
#define BOOLEANSATISFIABILITYPROBLEM_TABLE_H

#include <vector>
#include <tuple>
#include <iostream>
#include <any>

namespace pdp {
    template<class ...T_s>
    class table final {
        // friend std::ostream &operator<<(std::ostream& os, const table& table){ }
    public:
        typedef std::tuple<T_s...> Row;

    private:
        std::vector<Row> _rows;
        static constexpr int _n_cols = sizeof...(T_s);
        const std::array<std::string, _n_cols> _col_names;

    public:
        table() = default;

        explicit table(const std::array<std::string, _n_cols>& col_names)
                :_col_names{col_names}
        { }

        void add_row(const Row& row)
        {
            _rows.emplace_back(row);
        }

        constexpr int n_cols() const
        {
            return _n_cols;
        }

        const std::vector<Row>& rows() const
        {
            return _rows;
        }

        const std::array<std::string, _n_cols>& col_names() const
        {
            return _col_names;
        }

        const Row& row(int index) const
        {
            std::array<std::any, _n_cols> row;
        }
    };
}
#endif //BOOLEANSATISFIABILITYPROBLEM_TABLE_H
