#include "bnf-scanner.hpp"

namespace bnf {
    using namespace scanner;
    namespace {
        std::string scan_tag_and_consume_angle(parser &ctx) {
            std::string name;
            do {
                if (!name.empty())
                    name.push_back(' ');
                name = ctx.id_consume();
            } while (!ctx.try_to_consume(tag::ANGLE_RIGHT));
            return name;
        }

        rule_ptr scane_expression(parser &ctx);
        rule_ptr scane_group(parser &ctx) {
            rules list;
            do {
                rule_ptr item;
                if (ctx.try_to_consume(tag::ANGLE_LEFT)) {
                    auto name = scan_tag_and_consume_angle(ctx);
                    item = rule::create_rule(name);
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

    rule_map scan_bnf(parser &ctx) {
        rule_map map;
        while (!ctx.compare(none)) {
            // <rule>
            ctx.consume(tag::ANGLE_LEFT);
            auto name = scan_tag_and_consume_angle(ctx);
            if (is_exists(map, name))
                ctx.raise("dublicate name '{}'", name);
            // ::=
            ctx.consume(tag::ASSIGMENT);
            // expression
            map[to_upper(name)] = scane_expression(ctx);
            ctx.consume(tag::SEMICOLON);
        }
        return map;
    }

    void check_bnf(const rule_map &map, const rule_ptr &rule) {
        switch (rule->type) {
        case RULE: {
            if (!is_exists(map, rule->str()))
                throw std::runtime_error(fmt::format("symbol '{}' not found", rule->str()));
            rule->set_index(get_rule(map, rule->str())->index);
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