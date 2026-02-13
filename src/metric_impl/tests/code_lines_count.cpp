#include "metric_impl/code_lines_count.hpp"
#include <file.hpp>

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {
using namespace std::literals;

void CommonCodeLinesCountTest(const std::string &file_name, int etalon_value) {
    function::Function test_func;
    file::File file{file_name};
    test_func.ast = file.ast;
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<int>(result.value), etalon_value);
}

TEST(CodeLinesCountTest, Comments) { CommonCodeLinesCountTest("comments.py"s, 3); }

TEST(CodeLinesCountTest, Exception) { CommonCodeLinesCountTest("exceptions.py"s, 8); }

TEST(CodeLinesCountTest, If) { CommonCodeLinesCountTest("if.py"s, 3); }

TEST(CodeLinesCountTest, Loops) { CommonCodeLinesCountTest("loops.py"s, 6); }

TEST(CodeLinesCountTest, Lines) { CommonCodeLinesCountTest("many_lines.py"s, 11); }

TEST(CodeLinesCountTest, ManyParameters) { CommonCodeLinesCountTest("many_parameters.py"s, 1); }

TEST(CodeLinesCountTest, MatchCase) { CommonCodeLinesCountTest("match_case.py"s, 7); }

TEST(CodeLinesCountTest, NestedIf) { CommonCodeLinesCountTest("nested_if.py"s, 9); }

TEST(CodeLinesCountTest, Simple) { CommonCodeLinesCountTest("simple.py"s, 5); }

TEST(CodeLinesCountTest, Ternary) { CommonCodeLinesCountTest("ternary.py"s, 1); }

}  // namespace analyser::metric::metric_impl
