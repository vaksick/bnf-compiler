/// Copyright (c) 2020 Viktor Lazarev 
//! @version 0.1
//! @author vaksick@gmail.com 

#pragma once
#include "bnf-notation.hpp"
#include "parser.hpp"
#include <map>

namespace bnf {
    rule_map scan_bnf(scanner::parser &ctx);

    void check_bnf(const rule_map &map);
} // namespace bnf
