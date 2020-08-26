#include "output-xml.hpp"
#include <iostream>
#include <sstream>

const std::string space_tab(2, ' ');

namespace bnf::xml {
    void print(std::ostream &os, tree_ptr tree);
    std::ostream &operator<<(std::ostream &os, std::stringstream &ss) {
        std::string line;
        while (std::getline(ss, line))
            os << space_tab << line << std::endl;
        return os;
    }

    void print(std::ostream &os, const data_t &value) {
        if (is_null(value)) {
            os << "<tag:null />" << std::endl;
        } else if (is_string(value)) {
            os << "<tag:string>";
            os << get_string(value);
            os << "</tag:string>" << std::endl;
        } else {
            print(os, get_tree(value));
        }
    }

    void print(std::ostream &os, const std::string &name, const data_t &value) {
        if (is_string(value)) {
            os << "<" << name << ">";
            os << get_string(value);
            os << "</" << name << ">" << std::endl;
        } else {
            os << "<" << name << ">" << std::endl;
            std::stringstream fake_io(std::ios_base::out | std::ios_base::in);
            print(fake_io, get_tree(value));
            os << fake_io;
            os << "</" << name << ">" << std::endl;
        }
    }

    void print(std::ostream &os, tree_ptr tree) {
        if (tree) {
            if (tree->is_null()) {
                os << "<tag:null />";
            } else if (tree->is_data()) {
                print(os, tree->name, tree->data());
            } else {
                os << "<" << tree->name << ">" << std::endl;
                std::stringstream fake_io(std::ios_base::out | std::ios_base::in);
                for (auto item : tree->list()) {
                    print(fake_io, item);
                }
                os << fake_io;
                os << "</" << tree->name << ">" << std::endl;
            }
        }
    }
} // namespace bnf::xml