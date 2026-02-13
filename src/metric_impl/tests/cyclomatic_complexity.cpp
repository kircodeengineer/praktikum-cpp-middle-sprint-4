#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::literals;

void CommonCyclomaticComplexityTest(const std::string &file_name, int etalon_value) {
    function::Function test_func;
    file::File file{file_name};
    test_func.ast = file.ast;
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<int>(result.value), etalon_value);
}

TEST(CyclomaticComplexityTest, Comments) { CommonCyclomaticComplexityTest("comments.py"s, 1); }

TEST(CyclomaticComplexityTest, Exception) { CommonCyclomaticComplexityTest("exceptions.py"s, 4); }

TEST(CyclomaticComplexityTest, If) { CommonCyclomaticComplexityTest("if.py"s, 2); }

TEST(CyclomaticComplexityTest, Loops) { CommonCyclomaticComplexityTest("loops.py"s, 4); }

TEST(CyclomaticComplexityTest, Lines) { CommonCyclomaticComplexityTest("many_lines.py"s, 2); }

TEST(CyclomaticComplexityTest, ManyParameters) { CommonCyclomaticComplexityTest("many_parameters.py"s, 2); }

TEST(CyclomaticComplexityTest, MatchCase) { CommonCyclomaticComplexityTest("match_case.py"s, 4); }

TEST(CyclomaticComplexityTest, NestedIf) { CommonCyclomaticComplexityTest("nested_if.py"s, 4); }

TEST(CyclomaticComplexityTest, Simple) { CommonCyclomaticComplexityTest("simple.py"s, 2); }

TEST(CyclomaticComplexityTest, Ternary) { CommonCyclomaticComplexityTest("ternary.py"s, 3); }

}  // namespace analyser::metric::metric_impl
