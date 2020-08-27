/// Copyright (c) 2020 Viktor Lazarev 
//! @version 0.1
//! @author vaksick@gmail.com 

#pragma once
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#define ENTRY_NAME "entry"

namespace bnf {
    enum rule_type { STR, RULE, GROUP, CHOICE, ARRAY, XOR_ARRAY };
    enum class variable_t { NONE, LIST, OPT, ONE_OR_MORE };

    struct rule;
    using rule_ptr = std::shared_ptr<rule>;
    using rules = std::vector<rule_ptr>;
    using value_t = std::variant<std::string, rules>;

    class rule {
    public:
        rule_type type;
        variable_t variable;
        value_t value;
        bool is_join;
        mutable uint64_t index; // internal use;
        rule(rule_type, const value_t &, bool is_join);

        const std::string &str() const;
        const rules &list() const;
        static rule_ptr create_str(const std::string &);
        static rule_ptr create_rule(const std::string &, bool is_join);
        static rule_ptr create_choice(const rules &);
        static rule_ptr create_array(const std::string &);
        static rule_ptr create_xor_array(const std::string &);
        static rule_ptr create_group(const rules &);

        bool allowed_empty() const {
            return variable == variable_t::LIST || variable == variable_t::OPT;
        }

        void set_index(uint64_t index);
    };

    using rule_map = std::map<std::string, rule_ptr>;

    std::string to_upper(std::string value);
    bool is_exists(const rule_map &map, const std::string &name);
    const rule_ptr &get_rule(const rule_map &map, const std::string &name);

} // namespace bnf
