#include "metric_impl/code_lines_count.hpp"
#include <file.hpp>

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {
using namespace std::literals;

void CommonTest(const std::string &file_name, int etalon_value) {
    function::Function test_func;
    analyser::file::File file{file_name};
    test_func.ast = file.ast;
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<int>(result.value), etalon_value);
}

TEST(CodeLinesCountTest, CountsCodeLinesComments) { CommonTest("comments.py"s, 3); }

TEST(CodeLinesCountTest, CountsCodeLinesException) { CommonTest("exceptions.py"s, 8); }

TEST(CodeLinesCountTest, CountsCodeLinesIf) { CommonTest("if.py"s, 3); }

TEST(CodeLinesCountTest, CountsCodeLinesLoops) { CommonTest("loops.py"s, 6); }

TEST(CodeLinesCountTest, CountsCodeLinesManyLines) { CommonTest("many_lines.py"s, 11); }

TEST(CodeLinesCountTest, CountsCodeLinesManyParameters) { CommonTest("many_parameters.py"s, 1); }

TEST(CodeLinesCountTest, CountsCodeLinesMatchCase) { CommonTest("match_case.py"s, 7); }

TEST(CodeLinesCountTest, CountsCodeLinesNestedIf) { CommonTest("nested_if.py"s, 9); }

TEST(CodeLinesCountTest, CountsCodeLinesSimple) { CommonTest("simple.py"s, 5); }

TEST(CodeLinesCountTest, CountsCodeLinesTernary) { CommonTest("ternary.py"s, 1); }

}  // namespace analyser::metric::metric_impl
