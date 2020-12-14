#include <gtest/gtest.h>
using namespace testing;

#include "Counter.gtest-verilog.h"
using namespace counter;

namespace {
    class Counter : public ::testing::Test {
    public:
        void SetUp() override {
        }
        
        void TearDown() override {
        }

        ::counter::TestBench testBench;
    };
}

TEST_F(Counter, ShouldConstruct) {
}

TEST_F(Counter, ShouldReset) {
    testBench.reset();

    // todo: use this to test Traces with multi-bit values
    ASSERT_EQ(testBench.core().o_value, 0);
}

TEST_F(Counter, ShouldIncrement) {
    testBench.reset();
    testBench.tick();

    // todo: use this to test Traces with multi-bit values
    ASSERT_EQ(testBench.core().o_value, 1);
}

TEST_F(Counter, ShouldIncrementRepeatedly) {
    testBench.reset();
    testBench.tick(10);

    // todo: use this to test Traces with multi-bit values
    ASSERT_EQ(testBench.core().o_value, 10);
}