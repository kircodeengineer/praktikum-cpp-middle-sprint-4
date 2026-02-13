#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::literals;

void CyclomaticComplexityMetricTest(const std::string &file_name, int etalon_value) {
    function::Function test_func;
    file::File file{file_name};
    test_func.ast = file.ast;
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<int>(result.value), etalon_value);
}

TEST(CyclomaticComplexityMetricTest, Comments) { CyclomaticComplexityMetricTest("comments.py"s, 1); }

TEST(CyclomaticComplexityMetricTest, Exception) { CyclomaticComplexityMetricTest("exceptions.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, If) { CyclomaticComplexityMetricTest("if.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, Loops) { CyclomaticComplexityMetricTest("loops.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, Lines) { CyclomaticComplexityMetricTest("many_lines.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, ManyParameters) { CyclomaticComplexityMetricTest("many_parameters.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, MatchCase) { CyclomaticComplexityMetricTest("match_case.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, NestedIf) { CyclomaticComplexityMetricTest("nested_if.py"s, 4); }

TEST(CyclomaticComplexityMetricTest, Simple) { CyclomaticComplexityMetricTest("simple.py"s, 2); }

TEST(CyclomaticComplexityMetricTest, Ternary) { CyclomaticComplexityMetricTest("ternary.py"s, 3); }

}  // namespace analyser::metric::metric_impl
