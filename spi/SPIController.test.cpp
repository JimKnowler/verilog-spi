#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "spi/SPIControllerTestBench.h"
using namespace spicontrollertestbench;

namespace {
    class SPIController : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();
            testBench.trace.clear();
        }

        SPIControllerTestBench testBench;
    };
}

TEST_F(SPIController, ShouldConstructFixture) {
}

TEST_F(SPIController, ShouldReset) {
    testBench.reset();

    const Trace expectedTrace = TraceBuilder()
        .port(o_tx_ready).signal("0")
        .port(o_spi_clk).signal("0")
        .port(o_spi_copi).signal("0")
        .allPorts().repeat(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldReportReadyToTransmit) {
    testBench.tick(10);           // give core a clock cycle to settle after reset
    
    const Trace expectedTrace = TraceBuilder()
        .port(o_tx_ready).signal("1")
        .port(o_spi_clk).signal("0")
        .port(o_spi_copi).signal("0")
        .allPorts().repeat(20);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldIdleSpiClockWhileIdle) {
    testBench.tick(50);

    const Trace expectedTrace = TraceBuilder()
        .port(o_spi_clk).signal("00").repeat(50);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldSetupSend) {
    auto& core = testBench.core();
    
    // setup a tx by pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xFF;
    testBench.tick();

    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(      "10" )
        .port(o_tx_ready).signal( "00" )      // should be 0 during setup
        .port(o_spi_clk).signal(  "00" )      // should not be pulsed during setup
        .port(o_spi_copi).signal( "00" );     // should be 0 during setup
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldSendByte0xFF) { 
    auto& core = testBench.core();
    
    // setup a tx by pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xFF;
    testBench.tick();
    
    // reset trace, so we only capture signals during the transmission
    testBench.trace.clear();

    // send in progress
    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    testBench.tick(8 * 2);
    
    const Trace expectedTrace = TraceBuilder()
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
    testBench.trace.clear();

    // send in progress
    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    testBench.tick(8 * 2);
    
    const Trace expectedTrace = TraceBuilder()
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
    testBench.trace.clear();

    // send in progress
    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    testBench.tick(8 * 2);
    
    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(      "10101010" )
        .port(o_tx_ready).signal( "00000000" )      // should be 0 while sending
        .port(o_spi_clk).signal(  "11001100" )      // should be pulsed every other tick while sending
        .port(o_spi_copi).signal( "00001111" )      // should alternate 0,1 while sending
        .allPorts().repeat(4);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

// send and receive byte - when is o_RX_DV pulsed? when does o_TX_Ready go high?
//

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
//           -> via parameter to verilog module