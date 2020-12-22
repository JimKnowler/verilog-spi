#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "spi/SPIPeripheralTestBench.h"
using namespace spiperipheraltestbench;

namespace {
    class SPIPeripheral : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();
            testBench.trace.clear();
        }

        SPIPeripheralTestBench testBench;
    };
}

TEST_F(SPIPeripheral, ShouldConstructFixture) {
}

TEST_F(SPIPeripheral, ShouldReset) {
    testBench.reset();

    const Trace expectedTrace = TraceBuilder()
        .port(o_temp).signal("0")
        .allPorts().repeat(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}


// TODO: parameterised MODE
//       -> via parameter to verilog module
// - does o_spi_clk idle to high or low?
// - is data set at rising/falling edge?
// - is data sampled at rising/falling edge?

// TODO: parameterised CLKS_PER_HALF_BIT
//        -> via parameter to verilog module