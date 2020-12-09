#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MatchesTrace.h"
#include "TraceBuilder.h"

#include "TestBenchSPIController.h"

using namespace testing;

namespace {
    class SPIController : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();
            testBench.trace.clear();
        }

        TestBenchSPIController testBench;
    };
}

TEST_F(SPIController, ShouldConstructFixture) {
}

TEST_F(SPIController, ShouldReportReadyToTransmit) {
    ASSERT_EQ(1, testBench.core().o_tx_ready);
}

TEST_F(SPIController, ShouldIdleSpiClockWhileIdle) {
    testBench.tick(50);

    Trace expected = TraceBuilder()
        .port(o_spi_clk).signal("00").repeat(50);

    EXPECT_THAT(testBench.trace, MatchesTrace(expected));
}

TEST_F(SPIController, ShouldSendByteF) { 
    auto& core = testBench.core();
    
    // start sending 0xf by pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xf;
    testBench.tick();
    
    // reset trace, so we only capture signals during the transmission
    // TODO: or, should we include the first tick in Trace, to make sure that
    //       SPI lines are idle?
    // IDEA: support concatenating Traces 
    testBench.trace.clear();

    // send in progress
    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    testBench.tick(8 * 2);

    // o_tx_ready should be 0 while sending
    // o_spi_clk should be pulsed every other tick while sending
    // o_spi_copi should be 1 while sending
    
    Trace expectedTrace = TraceBuilder()
             .port(i_clk).signal("1010")
        .port(o_tx_ready).signal("0000")
         .port(o_spi_clk).signal("1100")
        .port(o_spi_copi).signal("1111")
        .allPorts().repeat(8);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

// send 0b10101010
// send 0b01010101


// send and receive byte - when is o_RX_DV pulsed? when does o_TX_Ready go high?
// 

// note: assume single clock cycle per half tick
// note: helper to tick + sample outputs at rising/falling edge

// send TX byte
//   cache TX byte when starting to write
// receive RX byte
//   data from peripheral
//   pulse o_rx_dv for a single clock cycle
// idle clock after send/receive has finished
//   SPI modes - is clock idle high or low?
// o_tx_ready - should it return to high immediately after sending a byte to peripheral, or after receiving a byte from the peripheral?
// configure the number of bytes we expect back from the peripheral (is this always 1?)

// TODO - parameterised CLKS_PER_HALF_BIT
// TODO - parameterised SPI mode

// todo: reset - check trace