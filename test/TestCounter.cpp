#include <gtest/gtest.h>
#include "VCounter.h"
#include "TestBench.h"

namespace {
    class CounterTest : public ::testing::Test {
    public:
        void SetUp() override {
        }
        
        void TearDown() override {
        }

        TestBench<VCounter> testBench;
    };
}

TEST_F(CounterTest, ShouldConstruct) {
}

TEST_F(CounterTest, ShouldReset) {
    testBench.reset();
    ASSERT_EQ(testBench.core().o_value, 0);
}

TEST_F(CounterTest, ShouldIncrement) {
    testBench.reset();
    testBench.tick();
    ASSERT_EQ(testBench.core().o_value, 1);
}

TEST_F(CounterTest, ShouldIncrementRepeatedly) {
    testBench.reset();
    testBench.tick(10);
    ASSERT_EQ(testBench.core().o_value, 10);
}