/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#include "input-file.hpp"
#include <fstream>
#include <streambuf>
#include <fmt/format.h>

namespace utils {
    common::input_ptr open(const std::string &name) {
        auto file = fopen(name.c_str(), "rb");
        if (!file)
            throw std::runtime_error(fmt::format("[open] don't open file '{}'", name));
        fseek(file, 0, SEEK_END);
        auto size = ftell(file);
        rewind(file);
        std::shared_ptr<char> mem(new char[size + 4]);
        fread(mem.get(), 1, size, file);
        memset(mem.get() + size, 0, 4);
        if (ferror(file)) {
            fclose(file);
            throw std::runtime_error(fmt::format("[open] don't read file '{}'", name));
        }
        fclose(file);
        return std::make_shared<common::input<char>>(mem);
    }
} // namespace utils