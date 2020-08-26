#include "bnf-notation.hpp"
#include <algorithm>

namespace bnf {

    static uint64_t g_index = 1;

    rule::rule(rule_type type, variable_t var, const value_t &value)
        : type(type), variable(var), value(value), index(++g_index) {
    }

    const std::string &rule::str() const {
        return std::get<std::string>(value);
    }
    const rules &rule::list() const {
        return std::get<rules>(value);
    }

    rule_ptr rule::create_str(const std::string &value, variable_t var) {
        return std::make_shared<rule>(STR, var, value);
    }
    rule_ptr rule::create_rule(const std::string &value, variable_t var) {
        return std::make_shared<rule>(RULE, var, value);
    }
    rule_ptr rule::create_choice(const rules &value, variable_t var) {
        return std::make_shared<rule>(CHOICE, var, value);
    }
    rule_ptr rule::create_group(const rules &value, variable_t var) {
        return std::make_shared<rule>(GROUP, var, value);
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
