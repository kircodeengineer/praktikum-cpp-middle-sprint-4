#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::literals;

void CommonNamingStyleMetricTest(const std::string &file_name, const std::string &etalon_value) {
    function::Function test_func;
    file::File file{file_name};
    test_func.ast = file.ast;
    NamingStyleMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<std::string>(result.value), etalon_value);
}

TEST(NamingStyleMetricTest, Comments) { CommonNamingStyleMetricTest("comments.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, Exception) { CommonNamingStyleMetricTest("exceptions.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, If) { CommonNamingStyleMetricTest("if.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, Loops) { CommonNamingStyleMetricTest("loops.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, Lines) { CommonNamingStyleMetricTest("many_lines.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, ManyParameters) { CommonNamingStyleMetricTest("many_parameters.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, MatchCase) { CommonNamingStyleMetricTest("match_case.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, NestedIf) { CommonNamingStyleMetricTest("nested_if.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, Simple) { CommonNamingStyleMetricTest("simple.py"s, "Lower Case"); }

TEST(NamingStyleMetricTest, Ternary) { CommonNamingStyleMetricTest("ternary.py"s, "Lower Case"); }

}  // namespace analyser::metric::metric_impl
