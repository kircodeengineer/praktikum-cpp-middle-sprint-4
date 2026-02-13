#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(AverageAccumulatorTest, BasicFunctionality) {
    AverageAccumulator accumulator;
    accumulator.Accumulate(metric::MetricResult{"", 10});
    accumulator.Accumulate(metric::MetricResult{"", 20});
    accumulator.Accumulate(metric::MetricResult{"", 30});

    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 20.0);
}

TEST(AverageAccumulatorTest, SingleValue) {
    AverageAccumulator accumulator;
    accumulator.Accumulate(metric::MetricResult{"", 42});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 42.0);
}

TEST(AverageAccumulatorTest, ResetFunctionality) {
    AverageAccumulator accumulator;
    accumulator.Accumulate(metric::MetricResult{"", 10});
    accumulator.Accumulate(metric::MetricResult{"", 20});

    accumulator.Reset();

    accumulator.Accumulate(metric::MetricResult{"", 5});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 5.0);
}

TEST(AverageAccumulatorTest, GetBeforeFinalize) {
    AverageAccumulator accumulator;
    accumulator.Accumulate(metric::MetricResult{"", 10});

    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(AverageAccumulatorTest, ZeroCount) {
    AverageAccumulator accumulator;
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 0.0);
}

TEST(AverageAccumulatorTest, MultipleAccumulate) {
    AverageAccumulator accumulator;
    std::ranges::for_each(std::views::iota(1, 11),
                          [&accumulator](int i) { accumulator.Accumulate(metric::MetricResult{"", i}); });

    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 5.5);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
