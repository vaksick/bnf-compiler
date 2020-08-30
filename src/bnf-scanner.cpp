/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#include "bnf-scanner.hpp"
#include <set>

namespace bnf {
    using namespace scanner;
    namespace {
        std::string scan_tag_and_consume_angle(parser &ctx) {
            std::string name;
            do {
                if (!name.empty())
                    name.push_back(' ');
                name += ctx.id_consume();
            } while (!ctx.try_to_consume(tag::ANGLE_RIGHT));
            return name;
        }

        char scan_char(parser &ctx) {
            auto str = ctx.str_consume();
            if (str.size() != 1)
                ctx.raise("array element must be one character");
            return str[0];
        }

        rule_ptr scane_expression(parser &ctx);
        rule_ptr scane_group(parser &ctx) {
            rules list;
            do {
                rule_ptr rule;
                if (ctx.try_to_consume(tag::ANGLE_LEFT)) {
                    auto prefix = ctx.array_consume(            //
                        {tag::PREFIX_JOIN, tag::PREFIX_HIDDEN}, //
                        {prefix_t::JOIN, prefix_t::HIDDEN},     //
                        prefix_t::NONE);
                    auto name = scan_tag_and_consume_angle(ctx);
                    rule = rule::create_rule(name, prefix);
                } else if (ctx.try_to_consume(tag::BRACKET_LEFT)) {
                    std::set<char> range;
                    bool is_not = ctx.try_to_consume(tag::NOT);
                    do {
                        auto chr = scan_char(ctx);
                        if (ctx.try_to_consume(tag::RANGE)) {
                            auto last = scan_char(ctx);
                            if (chr > last)
                                std::swap(chr, last);
                            for (; chr <= last; chr++)
                                range.insert(chr);
                        } else
                            range.insert(chr);
                    } while (ctx.try_to_consume(tag::COMMA));
                    ctx.consume(tag::BRACKET_RIGHT);
                    std::string value(range.begin(), range.end());
                    rule = is_not ? rule::create_xor_array(value) : rule::create_array(value);
                } else if (ctx.try_to_consume(tag::PARENT_LEFT)) {
                    rule = scane_expression(ctx);
                    ctx.consume(tag::PARENT_RIGHT);
                } else {
                    auto prefix = ctx.array_consume({tag::PREFIX_HIDDEN}, {prefix_t::HIDDEN}, prefix_t::NONE);
                    rule = rule::create_str(ctx.str_consume(), prefix);
                }
                if (ctx.try_to_consume(tag::PLUS)) {
                    rule->variable = variable_t::ONE_OR_MORE;
                } else if (ctx.try_to_consume(tag::IF)) {
                    rule->variable = variable_t::OPT;
                } else if (ctx.try_to_consume(tag::INDIRECTION)) {
                    rule->variable = variable_t::LIST;
                }
                list.push_back(rule);
            } while (!ctx.compare(tag::SEMICOLON, tag::PARENT_RIGHT, tag::CHOICE));
            if (list.size() == 1)
                return list[0];
            else
                return rule::create_group(list);
        }
        rule_ptr scane_expression(parser &ctx) {
            rules list;
            do {
                list.push_back(scane_group(ctx));
                if (ctx.compare(tag::PARENT_RIGHT))
                    break;
            } while (ctx.try_to_consume(tag::CHOICE));
            if (list.size() == 1)
                return list[0];
            else
                return rule::create_choice(list);
        }

    } // namespace

    rule_map scan_bnf(parser &ctx) {
        rule_map map;
        while (!ctx.compare(none)) {
            // <rule>
            ctx.consume(tag::ANGLE_LEFT);
            auto prefix = ctx.array_consume(            //
                {tag::PREFIX_JOIN, tag::PREFIX_HIDDEN}, //
                {prefix_t::JOIN, prefix_t::HIDDEN},     //
                prefix_t::NONE);
            auto name = scan_tag_and_consume_angle(ctx);
            if (is_exists(map, name))
                ctx.raise("dublicate name '{}'", name);
            // ::=
            ctx.consume(tag::ASSIGMENT);
            // expression
            auto rule = scane_expression(ctx);
            rule->prefix = prefix;
            map[to_upper(name)] = rule;
            ctx.consume(tag::SEMICOLON);
        }
        return map;
    }

    void check_bnf(const rule_map &map, const rule_ptr &rule) {
        switch (rule->type) {
        case RULE: {
            if (!is_exists(map, rule->str()))
                throw std::runtime_error(fmt::format("symbol '{}' not found", rule->str()));
            auto node = get_rule(map, rule->str());
            rule->set_index(node->index);
            if (rule->prefix == prefix_t::NONE)
                rule->prefix = node->prefix;
            break;
        }
        case GROUP:
        case CHOICE:
            for (auto node : rule->list())
                check_bnf(map, node);
            break;
        default:
            break;
        }
    }

    // infinite recursion

    void check_bnf(const rule_map &map) {
        if (!is_exists(map, ENTRY_NAME))
            throw std::runtime_error(fmt::format("rule '{}' not found ", ENTRY_NAME));
        for (auto pair : map)
            check_bnf(map, pair.second);
        // for (auto pair : map)
        //   check_bnf_infinite(pair.first, pair.second);
    }
} // namespace bnf