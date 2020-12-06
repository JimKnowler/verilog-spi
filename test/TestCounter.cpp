#include <gtest/gtest.h>
#include "VCounter.h"
#include "TestBench.h"

namespace {
    class TestCounter : public ::testing::Test {
    public:
        void SetUp() override {
        }
        
        void TearDown() override {
        }

        TestBench<VCounter> testBench;
    };
}

TEST_F(TestCounter, ShouldConstruct) {
}

TEST_F(TestCounter, ShouldReset) {
    testBench.reset();

    // todo: use this to test Traces with multi-bit values
    ASSERT_EQ(testBench.core().o_value, 0);
}

TEST_F(TestCounter, ShouldIncrement) {
    testBench.reset();
    testBench.tick();

    // todo: use this to test Traces with multi-bit values
    ASSERT_EQ(testBench.core().o_value, 1);
}

TEST_F(TestCounter, ShouldIncrementRepeatedly) {
    testBench.reset();
    testBench.tick(10);

    // todo: use this to test Traces with multi-bit values
    ASSERT_EQ(testBench.core().o_value, 10);
}