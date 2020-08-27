/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#include "parser-by-bnf.hpp"
#include <sstream>

namespace bnf {

    inline bool is_null(const data_t &value) {
        if (is_tree(value))
            return get_tree(value) == nullptr;
        if (is_string(value))
            return get_string(value).empty();
        return false;
    }
    inline bool is_string(const data_t &value) {
        return std::holds_alternative<std::string>(value);
    }
    inline bool is_tree(const data_t &value) {
        return std::holds_alternative<tree_ptr>(value);
    }
    inline const std::string &get_string(const data_t &value) {
        return std::get<std::string>(value);
    }
    inline const tree_ptr &get_tree(const data_t &value) {
        return std::get<tree_ptr>(value);
    }
    bool is_null(const variant_t &value) {
        if (std::holds_alternative<nullptr_t>(value))
            return true;
        if (is_data(value))
            return is_null(get_data(value));
        if (is_list(value))
            return get_list(value).size() == 0;
        return false;
    }
    bool is_string(const variant_t &value) {
        return is_data(value) && is_string(get_data(value));
    }
    bool is_tree(const variant_t &value) {
        return is_data(value) && is_tree(get_data(value));
    }
    bool is_data(const variant_t &value) {
        return std::holds_alternative<data_t>(value);
    }
    bool is_list(const variant_t &value) {
        return std::holds_alternative<std::vector<data_t>>(value);
    }
    const std::string &get_string(const variant_t &value) {
        return get_string(get_data(value));
    }
    const tree_ptr &get_tree(const variant_t &value) {
        return get_tree(get_data(value));
    }
    const data_t &get_data(const variant_t &value) {
        return std::get<data_t>(value);
    }
    const std::vector<data_t> &get_list(const variant_t &value) {
        return std::get<std::vector<data_t>>(value);
    }

    bool tree::is_null() const {
        return bnf::is_null(value);
    }
    bool tree::is_data() const {
        return bnf::is_data(value);
    }
    bool tree::is_list() const {
        return bnf::is_list(value);
    }
    const data_t &tree::data() const {
        return get_data(value);
    }
    const std::vector<data_t> &tree::list() const {
        return get_list(value);
    }
    tree::tree(const std::string &name) : name(name), value(nullptr) {
    }
    tree::tree(const std::string &name, const variant_t &value) : name(name), value(value) {
    }

    namespace scan {

        struct max_depth : public std::runtime_error {
            using std::runtime_error::runtime_error;
        };

        using input = common::input<char>;
        bool append(std::vector<data_t> &list, const variant_t &value) {
            if (is_null(value))
                return false;
            if (is_data(value)) {
                list.push_back(get_data(value));
                return true;
            } else if (is_list(value)) {
                for (auto element : get_list(value))
                    list.push_back(element);
                return true;
            }
            return false;
        }

        std::string join(const variant_t &value);
        std::string join(const std::vector<data_t> &list) {
            std::string result;
            for (auto item : list) {
                if (is_string(item)) {
                    result += get_string(item);
                } else if (is_tree(item)) {
                    result += join(get_tree(item));
                }
            }
            return result;
        }
        std::string join(const variant_t &value) {
            if (is_null(value))
                return std::string();
            if (is_string(value))
                return get_string(value);
            if (is_list(value))
                return join(get_list(value));
            if (is_tree(value))
                return join(get_tree(value)->value);
            return "- undefine - ";
        }
        class object {
        public:
            mutable input stream;
            const rule_map &map;

        public:
            const rule_ptr root;
            const std::string name;
            object(const input &file, const rule_map &map, const rule_ptr &root, const std::string &root_name)
                : stream(file), map(map), root(root), name(root_name) {
            }

            variant_t termal(const rule_ptr &rule, uint32_t depth) {
                auto pos = stream.getLocation();
                switch (rule->type) {
                case STR:
                    for (auto chr : rule->str()) {
                        if (stream.getChar() != chr)
                            throw bnf_error("[rule error] {}: expected string '{}'", pos, rule->str());
                        stream.shift(1);
                    }
                    return rule->str();
                case RULE: {
                    scan::object child(stream, map, rule /*root*/, rule->str());
                    auto value = child.expression(get_rule(map, rule->str()), depth);
                    if (is_null(value))
                        return nullptr;
                    stream = child.stream;
                    if (rule->is_join)
                        return std::make_shared<tree>(rule->str(), join(value));

                    return std::make_shared<tree>(rule->str(), value);
                }
                case GROUP: {
                    scan::object child(stream, map, root /*root*/, name + ".group");
                    auto value = child.range_expression(std::begin(rule->list()), std::end(rule->list()), depth);
                    if (is_null(value))
                        return nullptr;
                    stream = child.stream;
                    return value;
                }
                case CHOICE: {
                    for (auto node : rule->list()) {
                        scan::object child(stream, map, root /*root*/, name + ".choice");
                        auto value = child.try_expression(node, depth);
                        if (!is_null(value)) {
                            stream = child.stream;
                            return value;
                        }
                    }
                    throw bnf_error("[rule error] {}: failed to make a choice in tag:'{}'", pos, name);
                }
                case ARRAY: {
                    // only char
                    auto value = stream.getChar();
                    for (auto chr : rule->str()) {
                        if (chr == value) {
                            stream.shift();
                            return std::string(1, value);
                        }
                    }
                    return nullptr;
                }
                case XOR_ARRAY: {
                    // only char
                    auto value = stream.getChar();
                    for (auto chr : rule->str()) {
                        if (chr == value)
                            return nullptr;
                    }
                    stream.shift();
                    return std::string(1, value);
                }

                default:
                    throw bnf_error("[rule error] unknown rule type");
                }
            }
            variant_t range_expression(rules::const_iterator first, rules::const_iterator last, uint32_t depth) {
                if (first == last)
                    return nullptr;
                std::vector<data_t> result;
                if ((*first)->index == root->index) {
                    if (depth == 0) // fix infinite recursion
                        throw max_depth("infinite recursion");
                    auto value = try_expression(*(first++), depth - 1);
                    if (is_null(value)) // if null to return
                        return nullptr;
                    try {
                        scan::object child(stream, map, root, name); // save position;
                        auto value2 = child.range_expression(first, last, depth - 1);
                        if (is_null(value2)) // if null to return
                            return get_tree(value)->list();
                        stream = child.stream;
                        append(result, value);
                        append(result, value2);
                    } catch (...) {
                        return get_tree(value)->list();
                    }
                } else {
                    for (; first != last; ++first) {
                        auto value = expression(*first, depth);
                        if (is_null(value) && !(*first)->allowed_empty())
                            return nullptr;
                        append(result, value);
                    };
                }
                return result;
            }

            variant_t expression(const rule_ptr &rule, uint32_t depth) {
                std::vector<data_t> result;
                switch (rule->variable) {
                case variable_t::NONE:
                    return termal(rule, depth);
                case variable_t::OPT:
                    return try_termal(rule, depth);
                case variable_t::ONE_OR_MORE:
                    append(result, termal(rule, depth));
                    // break; //next rule
                case variable_t::LIST:
                    while (!stream.eof()) {
                        auto expt = try_termal(rule, depth);
                        if (is_null(expt) || !append(result, expt))
                            break;
                    }
                    return result;
                default:
                    throw std::runtime_error("NNN");
                }
            }

            variant_t try_termal(const rule_ptr &rule, uint32_t depth) {
                try {
                    return termal(rule, depth);
                } catch (...) {
                    return nullptr;
                }
            }

            variant_t try_expression(const rule_ptr &rule, uint32_t depth) {
                try {
                    return expression(rule, depth);
                } catch (const std::exception &e) {
                    // printf("%s\n", e.what());
                    return nullptr;
                }
            }
        };
    } // namespace scan

    tree_ptr scan_by_bnf(const input_ptr &file, const rule_map &map) {
        scan::object object(*file, map, get_rule(map, ENTRY_NAME), ENTRY_NAME);
        auto value = object.expression(object.root, 10);
        if (is_null(value))
            throw bnf_error("[rule error] {} is null", ENTRY_NAME);
        return std::make_shared<tree>(ENTRY_NAME, value);
    }
} // namespace bnf
