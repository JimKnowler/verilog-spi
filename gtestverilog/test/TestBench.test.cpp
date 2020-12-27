#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

namespace {
    class TestCore {
    public:
        void eval() {
            /// @todo test when eval is called
        }

        uint8_t i_clk;
        uint8_t i_reset_n;
    };
}

TEST(TestBench, ShouldConstruct) {
    TestBench<TestCore> testBench;
}

TEST(TestBench, ShouldReset) {
    TestBench<TestCore> testBench;
    testBench.reset();

    /// @todo test that underlying clk is set and eval'd
}

TEST(TestBench, ShouldTick) {
    TestBench<TestCore> testBench;
    testBench.tick();
}

TEST(TestBench, ShouldTickMultipleTimes) {
    TestBench<TestCore> testBench;
    testBench.tick(10);
}

TEST(TestBench, ShouldNextStep) {
    TestBench<TestCore> testBench;
    testBench.nextStep();
}

TEST(TestBench, ShouldInvokeOnStep) {
    class MyTestBench : public TestBench<TestCore> {
    public:
        int counter = 0;

        virtual void onNextStep() {
            counter += 1;
        }
    };

    MyTestBench testBench;

    testBench.nextStep();
    ASSERT_EQ(1, testBench.counter);

    testBench.nextStep();
    ASSERT_EQ(2, testBench.counter);
}

TEST(TestBench, ShouldAccessCore) {
    TestBench<TestCore> testBench;
    TestCore& core = testBench.core();
    ASSERT_NE(&core, nullptr);
}

TEST(TestBench, ShouldGetStepCount) {
    TestBench<TestCore> testBench;
    ASSERT_EQ(0, testBench.stepCount());

    testBench.nextStep();
    ASSERT_EQ(1, testBench.stepCount());

    testBench.nextStep();
    ASSERT_EQ(2, testBench.stepCount());

    testBench.tick(3);
    ASSERT_EQ(8, testBench.stepCount());
}