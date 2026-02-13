#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::literals;

void CommonCountParametersMetricTest(const std::string &file_name, int etalon_value) {
    function::Function test_func;
    file::File file{file_name};
    test_func.ast = file.ast;
    CountParametersMetric metric;
    auto result = metric.Calculate(test_func);
    EXPECT_EQ(std::get<int>(result.value), etalon_value);
}

TEST(CountParametersMetricTest, Comments) { CommonCountParametersMetricTest("comments.py"s, 3); }

TEST(CountParametersMetricTest, Exception) { CommonCountParametersMetricTest("exceptions.py"s, 0); }

TEST(CountParametersMetricTest, If) { CommonCountParametersMetricTest("if.py"s, 1); }

TEST(CountParametersMetricTest, Loops) { CommonCountParametersMetricTest("loops.py"s, 1); }

TEST(CountParametersMetricTest, Lines) { CommonCountParametersMetricTest("many_lines.py"s, 0); }

TEST(CountParametersMetricTest, ManyParameters) { CommonCountParametersMetricTest("many_parameters.py"s, 5); }

TEST(CountParametersMetricTest, MatchCase) { CommonCountParametersMetricTest("match_case.py"s, 1); }

TEST(CountParametersMetricTest, NestedIf) { CommonCountParametersMetricTest("nested_if.py"s, 2); }

TEST(CountParametersMetricTest, Simple) { CommonCountParametersMetricTest("simple.py"s, 0); }

TEST(CountParametersMetricTest, Ternary) { CommonCountParametersMetricTest("ternary.py"s, 1); }

}  // namespace analyser::metric::metric_impl
