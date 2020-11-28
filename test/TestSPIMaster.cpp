#include <gtest/gtest.h>
#include "VSPIMaster.h"
#include "TestBench.h"

namespace {
    class SPIMasterTest : public ::testing::Test {
    public:
        TestBench<VSPIMaster> testBench;
    };
}

TEST_F(SPIMasterTest, ShouldConstruct) {
}

TEST_F(SPIMasterTest, ShouldReset) {
    testBench.reset();
}
