/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#pragma once
#include <cstddef>
#include <fmt/format.h>
#include <memory>

namespace common {
    struct location {
        uint32_t line;
        uint32_t column;
        constexpr location(uint32_t l, uint32_t c) : line(l), column(c) {
        }
        inline bool operator==(const location &rvalue) const {
            return (line == rvalue.line) && (column == rvalue.column);
        }
        inline std::string str() const {
            return fmt::format("{}:{}", line, column);
        }
    };

    template <typename T>
    class input {
    private:
        std::shared_ptr<T> ptr;
        uint32_t line;
        uint32_t column;
        const T *str;

    public:
        input(const std::shared_ptr<T> &mem) : ptr(mem), line(ptr ? 1 : 0), column(ptr ? 1 : 0), str(ptr.get()) {
        }
        bool eof() const {
            return !(str && *str);
        }
        T getChar() const {
            return str ? *str : 0;
        }
        T getNextChar() const {
            return str ? *(str + 1) : 0;
        }
        T getNext2Char() const {
            return str ? *(str + 2) : 0;
        }
        void shift(int offset = 1) {
            while (*str && (offset--) > 0) {
                str++;
                switch (*str) {
                case '\n':
                    line++;
                    column = 1;
                    break;
                case '\r':
                    offset++; // skip symbol CR
                default:
                    column++;
                    break;
                }
            }
        }

        location getLocation() const {
            return location(line, column);
        }
    }; // namespace common

    using input_ptr = std::shared_ptr<input<char>>;
} // namespace common