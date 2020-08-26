/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#pragma once
#include <cstddef>
#include <optional>
#include <variant>

namespace common {
    template <typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
    struct traits {
        using type = T;
        static const char *__names[];
        static const size_t size;
        static constexpr int toInt(const type value) {
            return (int)value;
        }
        static constexpr const char *str(const type value) {
            return __names[toInt(value)];
        }
        static constexpr const char *str(size_t idx) {
            return (idx < size) ? __names[idx] : nullptr;
        }
    };

#ifdef TOKEN_CPP

#define ENUM_DECLARE(T, N, ...)                                                                                        \
    enum class T { __VA_ARGS__ };                                                                                      \
    template <>                                                                                                        \
    const char *traits<T>::__names[] = {ENUM_NAME_##N(__VA_ARGS__)};                                                   \
    template <>                                                                                                        \
    const size_t traits<T>::size = N

#else

#define ENUM_DECLARE(T, N, ...) enum class T { __VA_ARGS__ }

#endif

    ENUM_DECLARE(tag, 17,
                 NONE,          // null
                 ID,            //
                 VALUE,         //
                 ASSIGMENT,     // ::=
                 BRACKET_LEFT,  // [
                 BRACKET_RIGHT, // ]
                 PARENT_LEFT,   // (
                 PARENT_RIGHT,  // )
                 ANGLE_LEFT,    // <
                 ANGLE_RIGHT,   // >
                 RANGE,         // ..
                 SEMICOLON,     // ;
                 IF,            // ?
                 INDIRECTION,   // *
                 PLUS,          // +
                 CHOICE,        // |
                 PREFIX_JOIN    // s:
    );                          // namespace common

    inline const char *to_string(tag v) {
        return traits<tag>::str(v);
    }
} // namespace common