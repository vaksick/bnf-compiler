#pragma once
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <map>

#define ENTRY_NAME "entry"

namespace bnf {
    enum rule_type { STR, RULE, GROUP, CHOICE };
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
        mutable uint64_t index; // internal use;
        rule(rule_type, variable_t, const value_t &);

        const std::string &str() const;
        const rules &list() const;
        static rule_ptr create_str(const std::string &, variable_t = variable_t::NONE);
        static rule_ptr create_rule(const std::string &, variable_t = variable_t::NONE);
        static rule_ptr create_choice(const rules &, variable_t = variable_t::NONE);
        static rule_ptr create_group(const rules &, variable_t = variable_t::NONE);

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
