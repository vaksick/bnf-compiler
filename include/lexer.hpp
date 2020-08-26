/// Copyright (c) 2020 Viktor Lazarev 
//! @version 0.1
//! @author vaksick@gmail.com 

#pragma once
#include "input.hpp"
#include "token.hpp"
#include <stdexcept>
#include <string>
#include <tuple>

namespace scanner {

    using location = common::location;
    using tag = common::tag;
    constexpr tag none = tag::NONE;

    struct lexer_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
        template <typename... Args>
        constexpr lexer_error(const std::string &format, const Args &... args)
            : runtime_error(fmt::format(format, args...)) {
        }
    };

    class lexer {
    protected:
        void skip_space();
        virtual void skip_comment();
        std::string read_pattern();
        std::string read_quated_string();

    public:
        mutable common::input_ptr input;
        lexer(const common::input_ptr &input);
        virtual ~lexer() = default;
        std::tuple<tag, std::string, location> readToken();
    };
} // namespace scanner