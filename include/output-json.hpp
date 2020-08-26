/// Copyright (c) 2020 Viktor Lazarev 
//! @version 0.1
//! @author vaksick@gmail.com 

#pragma once
#include "bnf-notation.hpp"
#include "parser-by-bnf.hpp"

namespace bnf::json {
    void print(std::ostream &os, tree_ptr tree);
}