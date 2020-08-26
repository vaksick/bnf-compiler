#include "bnf-scanner.hpp"

namespace bnf {
    using namespace scanner;
    namespace {
        rule_ptr scane_expression(parser &ctx);
        rule_ptr scane_group(parser &ctx) {
            rules list;
            do {
                rule_ptr item;
                if (ctx.try_to_consume(tag::ANGLE_LEFT)) {
                    item = rule::create_rule(ctx.id_consume());
                    ctx.consume(tag::ANGLE_RIGHT);
                } else if (ctx.try_to_consume(tag::PARENT_LEFT)) {
                    item = scane_expression(ctx);
                    ctx.consume(tag::PARENT_RIGHT);
                } else {
                    item = rule::create_str(ctx.str_consume());
                }
                if (ctx.try_to_consume(tag::PLUS)) {
                    item->variable = variable_t::ONE_OR_MORE;
                } else if (ctx.try_to_consume(tag::IF)) {
                    item->variable = variable_t::OPT;
                } else if (ctx.try_to_consume(tag::INDIRECTION)) {
                    item->variable = variable_t::LIST;
                }
                list.push_back(item);
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

    bool is_exists(const rule_map &map, const std::string &name) {
        auto item = map.find(name);
        return item != map.end();
    }

    static uint64_t g_index = 1;

    rule_map scan_bnf(parser &ctx) {
        rule_map map;
        while (!ctx.compare(none)) {
            // <symbol>
            ctx.consume(tag::ANGLE_LEFT);
            auto name = ctx.id_consume();
            if (is_exists(map, name))
                ctx.raise("dublicate name '{}'", name);

            ctx.consume(tag::ANGLE_RIGHT);
            // ::=
            ctx.consume(tag::ASSIGMENT);
            // expression
            auto rule = scane_expression(ctx);
            rule->set_index(++g_index);
            map[name] = rule;
            ctx.consume(tag::SEMICOLON);
        }
        return map;
    }

    void check_bnf(const rule_map &map, const rule_ptr &rule) {
        switch (rule->type) {
        case RULE: {
            if (!is_exists(map, rule->str()))
                throw std::runtime_error(fmt::format("symbol '{}' not found", rule->str()));
            rule->set_index(map.at(rule->str())->index);
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

    /*void check_bnf_infinite(const std::string &name, const rule_ptr &rule) {
        switch (rule->type) {
        case RULE:
            if (rule->str() == name)
                throw std::runtime_error(fmt::format("symbol '{}' infinite recursion", name));
            break;
        case GROUP:
            for (auto node : rule->list()) {
                check_bnf_infinite(name, node);
                if (node->variable == variable_type::LIST || node->variable == variable_type::ONE_OR_NONE)
                    continue;
                break;
            }
            break;
        case CHOICE:
            for (auto tag : node->list())
                check_bnf_infinite(name, tag);
            break;
        default:
            break;
        }
    }*/

    // infinite recursion

    void check_bnf(const rule_map &map) {
        if (!is_exists(map, ENTRY_NAME))
            throw std::runtime_error(fmt::format("rule '{}' not found ", ENTRY_NAME));
        for (auto pair : map)
            check_bnf(map, pair.second);
        //for (auto pair : map)
         //   check_bnf_infinite(pair.first, pair.second);
    }
} // namespace bnf