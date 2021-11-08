#include <vector>

#include <gmock/gmock.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "spi/SPIPeripheralEchoTestBench.h"
using namespace spiperipheralechotestbench;

#include "Helpers.h"

namespace {
    class SPIPeripheralEcho : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.core().i_spi_cs_n = 1;

            testBench.reset();
            testBench.trace.clear();
        }

        SPIPeripheralEchoTestBench testBench;
    };
}

TEST_F(SPIPeripheralEcho, ShouldConstructFixture) {
}

TEST_F(SPIPeripheralEcho, ShouldSendAndReceive) {
    testBench.core().i_spi_cs_n = 0;
    
    int tx[4];
    tx[0] = Helpers::peripheralEchoSimulateReceiveAndSendByte(testBench, 1);
    tx[1] = Helpers::peripheralEchoSimulateReceiveAndSendByte(testBench, 4);
    tx[2] = Helpers::peripheralEchoSimulateReceiveAndSendByte(testBench, 10);
    tx[3] = Helpers::peripheralEchoSimulateReceiveAndSendByte(testBench, 0);

    EXPECT_EQ(tx[0], 0);
    EXPECT_EQ(tx[1], 1);
    EXPECT_EQ(tx[2], 4);
    EXPECT_EQ(tx[3], 10);
}
