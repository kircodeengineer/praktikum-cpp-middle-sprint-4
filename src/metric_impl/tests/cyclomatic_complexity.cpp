#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::literals;

void CommonCyclomaticComplexityMetricTest(const std::string &file_name, int etalon_value) {
    function::Function test_func;
    file::File file{file_name};
    test_func.ast = file.ast;
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<int>(result.value), etalon_value);
}

TEST(CyclomaticComplexityMetricTest, Comments) { CommonCyclomaticComplexityMetricTest("comments.py"s, 1); }

TEST(CyclomaticComplexityMetricTest, Exception) { CommonCyclomaticComplexityMetricTest("exceptions.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, If) { CommonCyclomaticComplexityMetricTest("if.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, Loops) { CommonCyclomaticComplexityMetricTest("loops.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, Lines) { CommonCyclomaticComplexityMetricTest("many_lines.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, ManyParameters) { CommonCyclomaticComplexityMetricTest("many_parameters.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, MatchCase) { CommonCyclomaticComplexityMetricTest("match_case.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, NestedIf) { CommonCyclomaticComplexityMetricTest("nested_if.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, Simple) { CommonCyclomaticComplexityMetricTest("simple.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, Ternary) { CommonCyclomaticComplexityMetricTest("ternary.py"s, 3); }

}  // namespace analyser::metric::metric_impl
