

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

#define EXPECT_MAP_EQ(expected, actual)                                                                                \
    EXPECT_EQ(expected.size(), actual.size());                                                                         \
    for (const auto &[key, val] : expected) {                                                                          \
        EXPECT_TRUE(actual.find(key) != actual.end());                                                                 \
        EXPECT_EQ(val, actual.at(key));                                                                                \
    }

class CategoricalAccumulatorTest : public ::testing::Test {
protected:
    CategoricalAccumulator accumulator;
};

TEST_F(CategoricalAccumulatorTest, AccumulateSingleValue) {
    metric::MetricResult result{"category_a", std::string("test")};
    accumulator.Accumulate(result);
    accumulator.Finalize();
    auto freq{accumulator.Get()};
    EXPECT_MAP_EQ((std::unordered_map<std::string, int>{{"test", 1}}), freq);
}

TEST_F(CategoricalAccumulatorTest, AccumulateMultipleSameValues) {
    metric::MetricResult result{"", std::string("cat")};
    accumulator.Accumulate(result);
    accumulator.Accumulate(result);
    accumulator.Accumulate(result);
    accumulator.Finalize();
    auto freq{accumulator.Get()};
    EXPECT_MAP_EQ((std::unordered_map<std::string, int>{{"cat", 3}}), freq);
}

TEST_F(CategoricalAccumulatorTest, AccumulateDifferentValues) {
    accumulator.Accumulate(metric::MetricResult{"", std::string("x")});
    accumulator.Accumulate(metric::MetricResult{"", std::string("y")});
    accumulator.Accumulate(metric::MetricResult{"", std::string("x")});
    accumulator.Finalize();
    auto freq{accumulator.Get()};
    EXPECT_MAP_EQ((std::unordered_map<std::string, int>{{"x", 2}, {"y", 1}}), freq);
}

TEST_F(CategoricalAccumulatorTest, FinalizeIdempotent) {
    accumulator.Accumulate(metric::MetricResult{"", std::string("z")});
    accumulator.Finalize();
    accumulator.Finalize();

    auto freq = accumulator.Get();
    EXPECT_MAP_EQ((std::unordered_map<std::string, int>{{"z", 1}}), freq);
}

TEST_F(CategoricalAccumulatorTest, ResetClearsDataAndState) {
    accumulator.Accumulate(metric::MetricResult{"", std::string("old")});
    accumulator.Finalize();
    accumulator.Reset();

    accumulator.Accumulate(metric::MetricResult{"", std::string("new")});
    accumulator.Finalize();

    auto freq = accumulator.Get();
    EXPECT_MAP_EQ((std::unordered_map<std::string, int>{{"new", 1}}), freq);
}

TEST_F(CategoricalAccumulatorTest, ResetBeforeFinalize) {
    accumulator.Accumulate(metric::MetricResult{"", std::string("temp")});
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", std::string("final")});
    accumulator.Finalize();

    auto freq = accumulator.Get();
    EXPECT_MAP_EQ((std::unordered_map<std::string, int>{{"final", 1}}), freq);
}

TEST_F(CategoricalAccumulatorTest, GetThrowsRuntimeErrorIfNotFinalized) {
    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
