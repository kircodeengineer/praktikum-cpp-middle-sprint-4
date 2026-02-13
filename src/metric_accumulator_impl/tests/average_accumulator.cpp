#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

class AverageAccumulatorTest : public ::testing::Test {
protected:
    AverageAccumulator accumulator;
};

TEST_F(AverageAccumulatorTest, BasicFunctionality) {
    accumulator.Accumulate(metric::MetricResult{"", 10});
    accumulator.Accumulate(metric::MetricResult{"", 20});
    accumulator.Accumulate(metric::MetricResult{"", 30});

    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 20.0);
}

TEST_F(AverageAccumulatorTest, SingleValue) {
    accumulator.Accumulate(metric::MetricResult{"", 42});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 42.0);
}

TEST_F(AverageAccumulatorTest, ResetFunctionality) {
    accumulator.Accumulate(metric::MetricResult{"", 10});
    accumulator.Accumulate(metric::MetricResult{"", 20});

    accumulator.Reset();

    accumulator.Accumulate(metric::MetricResult{"", 5});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 5.0);
}

TEST_F(AverageAccumulatorTest, GetBeforeFinalize) {
    accumulator.Accumulate(metric::MetricResult{"", 10});

    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST_F(AverageAccumulatorTest, ZeroCount) {
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 0.0);
}

TEST_F(AverageAccumulatorTest, MultipleAccumulate) {
    AverageAccumulator accumulator;
    std::ranges::for_each(std::views::iota(1, 11),
                          [&accumulator](int i) { accumulator.Accumulate(metric::MetricResult{"", i}); });

    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 5.5);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
