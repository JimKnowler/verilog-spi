#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
using namespace testing;

#include "gtest-verilog/MatchesTrace.h"
#include "gtest-verilog/TraceBuilder.h"

#include "SPIController.gtest-verilog.h"
using namespace spi_controller;

namespace {
    class SPIController : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();
            testBench.trace.clear();
        }

        ::spi_controller::TestBench testBench;
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

TEST_F(SPIController, ShouldSendByte0xFF) { 
    auto& core = testBench.core();
    
    // start sending by pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xFF;
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
    
    Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(      "1010" )
        .port(o_tx_ready).signal( "0000" )      // should be 0 while sending
        .port(o_spi_clk).signal(  "1100" )      // should be pulsed every other tick while sending
        .port(o_spi_copi).signal( "1111" )      // should be 1 while sending
        .allPorts().repeat(8);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldSendByte0xAA) { 
    // 0xAA => 0b10101010

    auto& core = testBench.core();
    
    // start sendingby pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xAA;
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
    
    Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(      "10101010" )
        .port(o_tx_ready).signal( "00000000" )      // should be 0 while sending
        .port(o_spi_clk).signal(  "11001100" )      // should be pulsed every other tick while sending
        .port(o_spi_copi).signal( "11110000" )      // should alternate 1, 0
        .allPorts().repeat(4);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}


TEST_F(SPIController, ShouldSendByte0x55) { 
    // 0x55 => 0b01010101
    
    auto& core = testBench.core();
    
    // start sending by pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0x55;
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
    
    Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(      "10101010" )
        .port(o_tx_ready).signal( "00000000" )      // should be 0 while sending
        .port(o_spi_clk).signal(  "11001100" )      // should be pulsed every other tick while sending
        .port(o_spi_copi).signal( "00001111" )      // should alternate 0,1 while sending
        .allPorts().repeat(4);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

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