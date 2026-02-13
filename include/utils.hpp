#pragma once

#include <charconv>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>

inline int ToInt(std::string_view value) {
    auto trimmed_value{value |
                       std::views::drop_while([](char c) { return std::isspace(static_cast<unsigned char>(c)); })};

    if (trimmed_value.empty())
        throw std::invalid_argument("Cannot convert empty string to integral");

    int result{};
    auto [parse_end_ptr, error_code] = std::from_chars(trimmed_value.begin(), trimmed_value.end(), result);
    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size()) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}
