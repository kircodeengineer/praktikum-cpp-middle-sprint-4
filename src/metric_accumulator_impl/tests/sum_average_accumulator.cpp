#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

class SumAverageAccumulatorTest : public ::testing::Test {
protected:
    SumAverageAccumulator accumulator;
};

TEST_F(SumAverageAccumulatorTest, AccumulateSingleValueGetReturnsCorrectResult) {
    accumulator.Accumulate(metric::MetricResult{"", 42});
    accumulator.Finalize();

    auto result{accumulator.Get()};
    EXPECT_EQ(42, result.sum);
    EXPECT_DOUBLE_EQ(42.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, AccumulateMultipleValuesGetReturnsCorrectResult) {
    accumulator.Accumulate(metric::MetricResult{"", 10});
    accumulator.Accumulate(metric::MetricResult{"", 20});
    accumulator.Accumulate(metric::MetricResult{"", 30});
    accumulator.Finalize();

    auto result{accumulator.Get()};
    EXPECT_EQ(60, result.sum);
    EXPECT_DOUBLE_EQ(20.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, AccumulateNegativeValuesGetReturnsCorrectResult) {
    accumulator.Accumulate(metric::MetricResult{"", -5});
    accumulator.Accumulate(metric::MetricResult{"", -10});
    accumulator.Finalize();

    auto result{accumulator.Get()};
    EXPECT_EQ(-15, result.sum);
    EXPECT_DOUBLE_EQ(-7.5, result.average);
}

TEST_F(SumAverageAccumulatorTest, AccumulateMixedValuesGetReturnsCorrectResult) {
    accumulator.Accumulate(metric::MetricResult{"", 100});
    accumulator.Accumulate(metric::MetricResult{"", -50});
    accumulator.Accumulate(metric::MetricResult{"", 25});
    accumulator.Finalize();

    auto result{accumulator.Get()};
    EXPECT_EQ(75, result.sum);
    EXPECT_DOUBLE_EQ(25.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, GetThrowsIfNotFinalized) { EXPECT_THROW(accumulator.Get(), std::runtime_error); }

TEST_F(SumAverageAccumulatorTest, ResetClearsStateGetThrowsBeforeFinalize) {
    accumulator.Accumulate(metric::MetricResult{"", 100});
    accumulator.Finalize();
    accumulator.Reset();

    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST_F(SumAverageAccumulatorTest, ResetBeforeFinalizeGetReturnsNewValues) {
    accumulator.Accumulate(metric::MetricResult{"", 99});
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Finalize();

    auto result{accumulator.Get()};
    EXPECT_EQ(1, result.sum);
    EXPECT_DOUBLE_EQ(1.0, result.average);
}
}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
