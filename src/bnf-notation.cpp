/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#include "bnf-notation.hpp"
#include <algorithm>

namespace bnf {

    static uint64_t g_index = 1;

    rule::rule(rule_type type, const value_t &value, bool is_join)
        : type(type), variable(variable_t::NONE), value(value), is_join(is_join), index(++g_index) {
    }

    const std::string &rule::str() const {
        return std::get<std::string>(value);
    }
    const rules &rule::list() const {
        return std::get<rules>(value);
    }

    rule_ptr rule::create_str(const std::string &value) {
        return std::make_shared<rule>(STR, value, false);
    }
    rule_ptr rule::create_rule(const std::string &value, bool is_join) {
        return std::make_shared<rule>(RULE, value, is_join);
    }
    rule_ptr rule::create_choice(const rules &value) {
        return std::make_shared<rule>(CHOICE, value, false);
    }
    rule_ptr rule::create_array(const std::string &value) {
        return std::make_shared<rule>(ARRAY, value, false);
    }
    rule_ptr rule::create_xor_array(const std::string &value) {
        return std::make_shared<rule>(XOR_ARRAY, value, false);
    }
    rule_ptr rule::create_group(const rules &value) {
        return std::make_shared<rule>(GROUP, value, false);
    }

    void rule::set_index(uint64_t value) {
        index = value;
    }

    std::string to_upper(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        return value;
    }

    bool is_exists(const rule_map &map, const std::string &name) {
        auto item = map.find(to_upper(name));
        return item != map.end();
    }

    const rule_ptr &get_rule(const rule_map &map, const std::string &name) {
        return map.at(to_upper(name));
    }

} // namespace bnf
