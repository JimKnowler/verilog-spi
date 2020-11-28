#include <gtest/gtest.h>
#include "VSPIController.h"
#include "TestBench.h"

namespace {
    class SPIControllerTest : public ::testing::Test {
    public:
        TestBench<VSPIController> testBench;
    };
}

TEST_F(SPIControllerTest, ShouldConstruct) {
}

TEST_F(SPIControllerTest, ShouldReset) {
    testBench.reset();
}

// assume single clock cycle 
// helper to tick + sample outputs at rising/falling edge

// send TX byte
// cache TX byte when starting to write
//  



// TODO - parameterised CLKS_PER_HALF_BIT