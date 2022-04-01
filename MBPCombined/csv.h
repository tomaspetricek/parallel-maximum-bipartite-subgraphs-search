//
// Created by Tomáš Petříček on 21.12.2021.
//

#ifndef BOOLEANSATISFIABILITYPROBLEM_CSV_H
#define BOOLEANSATISFIABILITYPROBLEM_CSV_H

#include <filesystem>
#include <fstream>
#include <utility>
#include "table.h"

namespace pdp {
    class csv final {
    private:
        const std::filesystem::path _path;
        const char _delim;
        std::ios_base::openmode _mode;

    public:
        explicit csv(std::filesystem::path path, char delim = ',') : _path{std::move(path)}, _delim{delim} {
            if (std::filesystem::exists(_path.string())) {
                _mode = std::ios_base::app;
            } else {
                _mode = std::ios_base::out;
            }
        }

        template<typename T>
        void write_element(std::ofstream& file, T el) {
            file << el;
        }

        template<typename T>
        void write_element(std::ofstream& file, std::vector<T> vec) {
            std::ostringstream oss;

            if (!vec.empty()) {
                std::copy(vec.begin(), vec.end() - 1,
                          std::ostream_iterator<T>(oss, " "));

                oss << vec.back();
            }

            file << oss.str();
        }

        template<int col_idx = 0, class ...T_s>
        void write_row(std::ofstream &file, const std::tuple<T_s...> &row) {
            write_element(file, std::get<col_idx>(row));

            if (col_idx < sizeof...(T_s) - 1) {
                file << _delim;
            } else {
                file << "\n";
            }

            if constexpr(col_idx + 1 != sizeof...(T_s)) {
                write_row<col_idx + 1>(file, row);
            }
        }

        template<int N, typename T>
        void write_row(std::ofstream &file, const std::array<T, N> &row) {
            for (int row_idx{0}; row_idx < row.size(); row_idx++) {
                file << row[row_idx];

                if (row_idx < row.size() - 1) {
                    file << _delim;
                } else {
                    file << "\n";
                }
            }
        }

        template<class ...T_s>
        void write(const pdp::table<T_s...> tab) {
            std::ofstream file{_path.string(), _mode};

            if (file.is_open()) {
                if (_mode != std::ios_base::app)
                    // write header
                    write_row<tab.n_cols(), std::string>(file, tab.col_names());

                for (const auto &row : tab.rows()) {
                    write_row(file, row);
                }
                file.close();
            } else {
                std::cerr << _path.string() << std::endl;
                throw std::runtime_error("Could not open file");
            }
        }

        void write(const std::filesystem::path &path, const std::vector<std::vector<std::string>> &rows) {
            const char delim{','};
            std::ofstream file{path.string()};

            if (file.is_open()) {
                for (const auto &row : rows) {
                    for (int idx{0}; idx < row.size(); idx++) {
                        file << row[idx];

                        if (idx < row.size() - 1)
                            file << delim;
                        else
                            file << "\n";
                    }
                }
                file.close();
            } else {
                std::cerr << path.string() << std::endl;
                throw std::runtime_error("Could not open file");
            }
        }
    };
}

#endif //BOOLEANSATISFIABILITYPROBLEM_CSV_H
