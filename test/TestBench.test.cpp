#include <gtest/gtest.h>

#include "TestBench.h"

#include "VCounter.h"

using namespace testing;

TEST(TestBench, ShouldConstruct) {
    TestBench<VCounter> testBench;
}

TEST(TestBench, ShouldReset) {
    TestBench<VCounter> testBench;
    testBench.reset();

    /// @todo test that underlying clk is set and eval'd
}

TEST(TestBench, ShouldTick) {
    TestBench<VCounter> testBench;
    testBench.tick();
}

TEST(TestBench, ShouldTickMultipleTimes) {
    TestBench<VCounter> testBench;
    testBench.tick(10);
}

TEST(TestBench, ShouldNextStep) {
    TestBench<VCounter> testBench;
    testBench.nextStep();
}

TEST(TestBench, ShouldInvokeOnStep) {
    class MyTestBench : public TestBench<VCounter> {
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
    TestBench<VCounter> testBench;
    VCounter& core = testBench.core();
    ASSERT_NE(&core, nullptr);
}

TEST(TestBench, ShouldGetStepCount) {
    TestBench<VCounter> testBench;
    ASSERT_EQ(0, testBench.stepCount());

    testBench.nextStep();
    ASSERT_EQ(1, testBench.stepCount());

    testBench.nextStep();
    ASSERT_EQ(2, testBench.stepCount());

    testBench.tick(3);
    ASSERT_EQ(8, testBench.stepCount());
}