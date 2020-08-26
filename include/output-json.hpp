#pragma once
#include "bnf-notation.hpp"
#include "parser-by-bnf.hpp"

namespace bnf::json {
    void print(std::ostream &os, tree_ptr tree);
}