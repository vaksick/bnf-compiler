#pragma once
#include "bnf-notation.hpp"
#include "parser.hpp"
#include <map>

namespace bnf {
    using rule_map = std::map<std::string, rule_ptr>;
    rule_map scan_bnf(scanner::parser &ctx);

    void check_bnf(const rule_map &map);
} // namespace bnf
