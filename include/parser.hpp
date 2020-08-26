#pragma once
#include "lexer.hpp"
#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <tuple>

namespace scanner {
    struct parser_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
        template <typename... Args>
        constexpr parser_error(const std::string &format, const Args &... args)
            : runtime_error(fmt::format(format, args...)) {
        }
    };

    struct parser {
    protected:
        std::tuple<tag, std::string, location> readToken();
        scanner::lexer lexer;

    public:
        parser(const common::input_ptr &input);
        virtual ~parser() = default;
        mutable tag currentTag;
        mutable std::string currentValue;
        mutable location position;
        void skip();
        bool compare(const tag &);
        template <typename... Tags>
        bool compare(const tag &value, Tags... list) {
            return compare(value) || compare(list...);
        }
        void consume(const tag &);
        bool try_to_consume(const tag &);

        std::string id_consume();
        bool try_to_id_consume(std::string &);

        std::string str_consume();
        bool try_to_str_consume(std::string &);

        void raiseEx(const location &loc, const std::string &message);
        void raise(const std::string &message);
        void raise_tag(const tag &);

        template <typename... Args>
        void raise(const std::string &format, const Args &... args) {
            raise(fmt::format(format, args...));
        }
    };
} // namespace scanner