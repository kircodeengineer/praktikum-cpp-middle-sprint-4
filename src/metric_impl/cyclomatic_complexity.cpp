#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include <print>
namespace analyser::metric::metric_impl {
std::string CyclomaticComplexityMetric::Name() const { return kName; }
MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    // Получаем строковое представление AST (абстрактного синтаксического дерева) функции.
    // Это S-выражение, сгенерированное утилитой tree-sitter, например:
    // "(function_definition name: (identifier) ... (if_statement ...) (for_statement ...))"
    auto &function_ast = f.ast;

    // Список типов узлов AST, каждый из которых увеличивает цикломатическую сложность на 1.
    // Эти узлы соответствуют управляющим конструкциям языка Python:
    // - if / elif
    // - циклы (for, while)
    // - обработка исключений (try, finally)
    // - case в match-выражениях
    // - assert
    // - тернарный оператор (conditional_expression)
    constexpr std::array<std::string_view, 9> complexity_nodes = {
        "if_statement",            // if
        "elif_statement",          // elif
        "for_statement",           // for
        "while_statement",         // while
        "try_statement",           // try
        "finally_clause",          // finally
        "case_clause",             // case
        "assert",                  // assert
        "conditional_expression",  // для тернарного оператора
    };
    auto all_tokens{complexity_nodes | std::views::transform([&](const auto &node) {
                        return function_ast | std::views::split(node) | std::views::drop(1);
                    }) |
                    std::views::join};

    auto count{std::ranges::distance(all_tokens)};
    return 1 + static_cast<int>(count);
}
}  // namespace analyser::metric::metric_impl
