/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#include "lexer.hpp"

namespace scanner {
    using namespace common;

    void lexer::skip_space() {
        while (!input->eof() && (isspace(input->getChar())))
            input->shift();
    }
    void lexer::skip_comment() {
        if ((input->getChar() == '/') && (input->getNextChar() == '*')) {
            input->shift(2);
            while (!input->eof() && !((input->getChar() == '*') && (input->getNextChar() == '/'))) {
                input->shift();
            }
            input->shift(2);
        }
    }
    std::string lexer::read_pattern() {
        std::string buffer;
        buffer.reserve(50); // reserve
        for (char c = input->getChar(); isalpha(c) || isdigit(c) || (c == '_'); c = input->getChar()) {
            buffer.push_back(c);
            input->shift();
        }
        return buffer;
    }
    std::string lexer::read_quated_string() {
        std::string buffer;
        buffer.reserve(255); // reserve
        char q = input->getChar();
        if ((q == '"') || (q == '\'')) {
            input->shift();
            for (char c = input->getChar(); c && (c != q); c = input->getChar()) {
                if (c == '\\') {
                    switch (tolower(input->getNextChar())) {
                    case 't':
                        buffer.push_back('\t');
                        break;
                    case 'r':
                        buffer.push_back('\r');
                        break;
                    case 'n':
                        buffer.push_back('\n');
                        break;
                    case 'v':
                        buffer.push_back('\v');
                        break;
                    case 'f':
                        buffer.push_back('\f');
                        break;
                    case '\\':
                        buffer.push_back('\\');
                        break;
                    case '\'':
                    case '"':
                        if (tolower(input->getNextChar()) == q)
                            buffer.push_back(q);
                        break;
                    default:
                        // error;
                        break;
                    }
                    input->shift(2);
                } else {
                    buffer.push_back(c);
                    input->shift();
                }
            }
            if (input->getChar() == q) {
                input->shift();
            }
        }
        return buffer;
    }
    lexer::lexer(const common::input_ptr &inp) : input(inp) {
        if (!input)
            throw lexer_error("input is empty");
    }
    std::tuple<tag, std::string, location> lexer::readToken() {
        if (!input)
            return std::make_tuple(none, "", location(0, 0));
        // Skip all spaces and comments
        while (true) {
            skip_space();
            if ((input->getChar() == '/') && (input->getNextChar() == '/')) {
                while (!input->eof() && (input->getChar() != '\n'))
                    input->shift();
                continue;
            }
            if ((input->getChar() == '/') && (input->getNextChar() == '*')) {
                skip_comment();
                continue;
            }
            break;
        }
        location pos(input->getLocation());
        // prefix ID
        if (input->getNextChar() == ':') {
            switch (input->getChar()) {
            case 's':
            case 'S':
                input->shift(2);
                return std::make_tuple(tag::PREFIX_JOIN, "", pos);
            }
        }

        // this is ID if first symbol alpha or _
        if (isalpha(input->getChar()) || (input->getChar() == '_')) {
            return std::make_tuple(tag::ID, read_pattern(), pos);
        } else {
            switch (input->getChar()) {
                // >
            case '>':
                input->shift();
                return std::make_tuple(tag::ANGLE_RIGHT, "", pos);
                // <
            case '<':
                input->shift();
                return std::make_tuple(tag::ANGLE_LEFT, "", pos);
                // [
            case '[':
                input->shift();
                return std::make_tuple(tag::BRACKET_LEFT, "", pos);
                // ]
            case ']':
                input->shift();
                return std::make_tuple(tag::BRACKET_RIGHT, "", pos);
                // (
            case '(':
                input->shift();
                return std::make_tuple(tag::PARENT_LEFT, "", pos);
                // )
            case ')':
                input->shift();
                return std::make_tuple(tag::PARENT_RIGHT, "", pos);
                // : ::=
            case ':':
                if (input->getNextChar() == ':' && input->getNext2Char() == '=') {
                    input->shift(3);
                    return std::make_tuple(tag::ASSIGMENT, "", pos);
                }
                break;
                // ;
            case ';':
                input->shift();
                return std::make_tuple(tag::SEMICOLON, "", pos);
                // +
            case '+':
                input->shift();
                return std::make_tuple(tag::PLUS, "", pos);
                // *
            case '*':
                input->shift();
                return std::make_tuple(tag::INDIRECTION, "", pos);
                // ?
            case '?':
                input->shift();
                return std::make_tuple(tag::IF, "", pos);
            case '|':
                input->shift();
                return std::make_tuple(tag::CHOICE, "", pos);
                // ,
            case ',':
                input->shift();
                return std::make_tuple(tag::COMMA, "", pos);
                // ^
            case '^':
                input->shift();
                return std::make_tuple(tag::NOT, "", pos);
                // . ..
            case '.':
                if (input->getNextChar() == '.') {
                    input->shift(2);
                    return std::make_tuple(tag::RANGE, "", pos);
                }
                break;
            case '"':
            case '\'':
                return std::make_tuple(tag::VALUE, read_quated_string(), pos);
            default:
                break;
            }
        }
        return std::make_tuple(none, "", pos);
    }
} // namespace scanner