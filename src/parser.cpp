/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#include "parser.hpp"

namespace scanner {
    using namespace common;

    parser::parser(const common::input_ptr &input) : lexer(input), currentTag(none), currentValue(""), position(0, 0) {
        std::tie(currentTag, currentValue, position) = readToken();
    }

    std::tuple<tag, std::string, location> parser::readToken() {
        return lexer.readToken();
    }

    void parser::skip() {
        consume(currentTag);
    }

    bool parser::compare(const tag &value) {
        return currentTag == value;
    }

    void parser::consume(const tag &value) {
        if (!try_to_consume(value))
            raise_tag(value);
    }

    bool parser::try_to_consume(const tag &value) {
        if (compare(value)) {
            // afore = std::move(curr);
            std::tie(currentTag, currentValue, position) = readToken();
            return true;
        }
        return false;
    }

    std::string parser::id_consume() {
        std::string out;
        if (!try_to_id_consume(out))
            raise_tag(tag::ID);
        return std::move(out);
    }

    bool parser::try_to_id_consume(std::string &out) {
        if (compare(tag::ID)) {
            out = currentValue;
            consume(currentTag);
            return true;
        }
        return false;
    }

    std::string parser::str_consume() {
        std::string out;
        if (!try_to_str_consume(out))
            raise_tag(tag::VALUE);
        return std::move(out);
    }

    bool parser::try_to_str_consume(std::string &out) {
        if (compare(tag::VALUE)) {
            out = currentValue;
            consume(currentTag);
            return true;
        }
        return false;
    }

    void parser::raiseEx(const location &loc, const std::string &message) {
        throw parser_error("[lexer error] {}: {}", loc.str(), message);
    }

    void parser::raise(const std::string &message) {
        raiseEx(position, message);
    }

    void parser::raise_tag(const tag &expected) {
        raise("expected tag: {}; factually tag: {}", to_string(expected), to_string(currentTag));
    }
} // namespace scanner