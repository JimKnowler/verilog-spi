#include <gtest/gtest.h>

namespace {
    class CounterTest : public ::testing::Test {
    public:
        void SetUp() override {
            printf("before the test\n");
        }
        
        void TearDown() override {
            printf("after the test\n");
        }
    };
}

TEST_F(CounterTest, ShouldRun) {
    EXPECT_EQ(1, 0);
}