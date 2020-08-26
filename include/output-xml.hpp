#pragma once
#include "bnf-notation.hpp"
#include "parser-by-bnf.hpp"

namespace bnf::xml {
    void print(std::ostream &os, tree_ptr tree);
}