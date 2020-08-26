#include "bnf-notation.hpp"

namespace bnf {

    rule::rule(rule_type type, variable_t var, const value_t &value) : type(type), variable(var), value(value), index(-1) {
    }

    const std::string & rule::str() const {
        return std::get<std::string>(value);
    }
    const rules & rule::list() const {
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

} // namespace bnf
