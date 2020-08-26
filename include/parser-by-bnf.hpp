/// Copyright (c) 2020 Viktor Lazarev 
//! @version 0.1
//! @author vaksick@gmail.com 

#pragma once
#include "bnf-notation.hpp"
#include "input.hpp"
#include <map>
#include <memory>
#include <vector>

namespace bnf {
    struct tree;
    using tree_ptr = std::shared_ptr<tree>;
    using data_t = std::variant<std::string, tree_ptr>;
    using variant_t = std::variant<nullptr_t, data_t, std::vector<data_t>>;

    bool is_null(const data_t &);
    bool is_null(const variant_t &);
    bool is_string(const data_t &);
    bool is_string(const variant_t &);
    bool is_tree(const data_t &);
    bool is_tree(const variant_t &);
    bool is_data(const variant_t &);
    bool is_list(const variant_t &);
    const std::string &get_string(const data_t &);
    const std::string &get_string(const variant_t &);
    const tree_ptr &get_tree(const data_t &);
    const tree_ptr &get_tree(const variant_t &);
    const data_t &get_data(const variant_t &);
    const std::vector<data_t> &get_list(const variant_t &);

    struct tree {
        std::string name;
        variant_t value;
        bool is_null() const;
        bool is_data() const;
        bool is_list() const;

        const data_t &data() const;
        const std::vector<data_t> &list() const;
        tree(const std::string &name);
        tree(const std::string &name, const variant_t &value);
    };

    struct bnf_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
        template <typename... Args>
        constexpr bnf_error(const std::string &format, const Args &... args)
            : runtime_error(fmt::format(format, args...)) {
        }
    };

    using common::input_ptr;
    using rule_map = std::map<std::string, rule_ptr>;
    tree_ptr scan_by_bnf(const input_ptr &input, const rule_map &map);
} // namespace bnf
