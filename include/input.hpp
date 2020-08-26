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

template <>
struct fmt::formatter<common::location> : formatter<fmt::string_view> {
    template <typename FormatContext>
    auto format(const common::location &loc, FormatContext &ctx) {
        return format_to(ctx.out(), "{}:{}", loc.line, loc.column);
    }
};