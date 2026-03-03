#include "metric_impl/code_lines_count.hpp"
#include <file.hpp>

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {
using namespace std::literals;

void CommonCodeLinesCountMetricTest(const std::string &file_name, int etalon_value) {
    function::Function test_func;
    file::File file{file_name};
    test_func.ast = file.ast;
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<int>(result.value), etalon_value);
}

TEST(CodeLinesCountMetricTest, Comments) { CommonCodeLinesCountMetricTest("comments.py"s, 3); }

TEST(CodeLinesCountMetricTest, Exception) { CommonCodeLinesCountMetricTest("exceptions.py"s, 8); }

TEST(CodeLinesCountMetricTest, If) { CommonCodeLinesCountMetricTest("if.py"s, 3); }

TEST(CodeLinesCountMetricTest, Loops) { CommonCodeLinesCountMetricTest("loops.py"s, 6); }

TEST(CodeLinesCountMetricTest, Lines) { CommonCodeLinesCountMetricTest("many_lines.py"s, 11); }

TEST(CodeLinesCountMetricTest, ManyParameters) { CommonCodeLinesCountMetricTest("many_parameters.py"s, 1); }

TEST(CodeLinesCountMetricTest, MatchCase) { CommonCodeLinesCountMetricTest("match_case.py"s, 7); }

TEST(CodeLinesCountMetricTest, NestedIf) { CommonCodeLinesCountMetricTest("nested_if.py"s, 9); }

TEST(CodeLinesCountMetricTest, Simple) { CommonCodeLinesCountMetricTest("simple.py"s, 5); }

TEST(CodeLinesCountMetricTest, Ternary) { CommonCodeLinesCountMetricTest("ternary.py"s, 1); }

}  // namespace analyser::metric::metric_impl
