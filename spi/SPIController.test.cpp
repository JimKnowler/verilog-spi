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

        /// @todo add testbench arg + move to helpers file
        void helperSetupSendByte(uint8_t byte) {
            auto& core = testBench.core();
            
            // start sending by pulsing command lines
            core.i_tx_dv = 1;
            core.i_tx_byte = byte;
            testBench.tick();
            
            // reset trace, so we only capture signals during the transmission
            testBench.trace.clear();

            // send in progress, clear the external inputs
            core.i_tx_dv = 0;
            core.i_tx_byte = 0;
        }

        /// @todo add testbench arg and move to helpers file
        void helperSimulateReceiveByte(uint8_t byte, uint32_t numStepsSetup = 0, uint32_t numStepsValid = 4, uint32_t numStepsPadding = 0) {
            auto& core = testBench.core();
            
            for (uint32_t index = 0; index < 8; index++) {
                core.i_spi_cipo = 0;
                for (uint32_t i=0; i<numStepsSetup; i++) {
                    testBench.nextStep();
                }

                core.i_spi_cipo = (byte >> (7-index)) & 0x1;                
                for (uint32_t i=0; i<numStepsValid; i++) {
                    testBench.nextStep();
                }

                core.i_spi_cipo = 0;
                
                for (uint32_t i=0; i<numStepsPadding; i++) {
                    testBench.nextStep();
                }
            }
        };

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
        .port(i_clk).signal("10")
        .port(o_tx_ready).signal("11")
        .port(o_spi_clk).signal("00")
        .port(o_spi_copi).signal("00")
        .allPorts().repeat(10);
    
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
    helperSetupSendByte(0xFF);
    
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
    helperSetupSendByte(0xAA);                      // 0xAA => 0b10101010

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
    helperSetupSendByte(0x55);                      // 0x55 => 0b01010101       

    testBench.tick(8 * 2);
    
    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(      "10101010" )
        .port(o_tx_ready).signal( "00000000" )      // should be 0 while sending
        .port(o_spi_clk).signal(  "11001100" )      // should be pulsed every other tick while sending
        .port(o_spi_copi).signal( "00001111" )      // should alternate 0,1 while sending
        .allPorts().repeat(4);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldReportTxReadyAfterSendComplete) {
    helperSetupSendByte(0x55);                      // 0x55 => 0b01010101       

    testBench.tick(8 * 2);
    testBench.trace.clear();

    testBench.tick(10);
    
    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(      "10" )
        .port(o_tx_ready).signal( "11" )
        .port(o_spi_clk).signal(  "00" )
        .port(o_spi_copi).signal( "00" )
        .allPorts().repeat(10);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldReceiveByte0x42) {
    helperSetupSendByte(0);

    helperSimulateReceiveByte(0x42);

    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(        "10101010" ).repeat(4)
        .port(i_spi_cipo).signal(   "01000010").repeatEachStep(4)
        .port(o_rx_dv).signal(      "0").repeat(4 * 8)
        .port(o_rx_byte).signal(    {0}       ).repeat(4 * 8);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldReportReceivedByte0x42) {
    helperSetupSendByte(0);
    helperSimulateReceiveByte(0x42);
    testBench.trace.clear();

    testBench.tick(2);

    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(        "1010" )
        .port(i_spi_cipo).signal(   "0000")
        .port(o_rx_dv).signal(      "1100")
        .port(o_rx_byte).signal(    {0x42, 0} ).repeatEachStep(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldReportReceivedByte0x55) {
    helperSetupSendByte(0);
    helperSimulateReceiveByte(0x55);                            // 0x55 => 0b01010101 
    testBench.trace.clear();

    testBench.tick(2);

    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(        "1010" )
        .port(i_spi_cipo).signal(   "0000")
        .port(o_rx_dv).signal(      "1100")
        .port(o_rx_byte).signal(    {0x55, 0} ).repeatEachStep(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldSampleControllerInOnFallingEdge) {
    helperSetupSendByte(0);
    helperSimulateReceiveByte(0x55,2,1,1);
    testBench.trace.clear();

    testBench.tick(2);

    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal(        "1010" )
        .port(i_spi_cipo).signal(   "0000")
        .port(o_rx_dv).signal(      "1100")
        .port(o_rx_byte).signal(    {0x55, 0} ).repeatEachStep(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIController, ShouldSendAndReceiveMultipleBytes) {
    // simulate sending first byte
    helperSetupSendByte(0x12);                      
    helperSimulateReceiveByte(0x34);
    testBench.tick(2);
    testBench.trace.clear();

    // simulate sending second byte
    helperSetupSendByte(0x55);                      // 0x55 => 0b01010101 
    helperSimulateReceiveByte(0xBB);
    testBench.tick(2);

    const Trace expectedSendTrace = TraceBuilder()
        .port(i_clk).signal(      "10101010" )
        .port(o_tx_ready).signal( "00000000" )      // should be 0 while sending
        .port(o_spi_clk).signal(  "11001100" )      // should be pulsed every other tick while sending
        .port(o_spi_copi).signal( "00001111" )      // should alternate 0,1 while sending
        .port(o_rx_dv).signal(    "00000000" )
        .port(o_rx_byte).signal(  {0} ).repeat(8)
        .allPorts().repeat(4);

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal(        "1010" )
        .port(o_tx_ready).signal(   "1111" )
        .port(o_spi_clk).signal(    "0000" )
        .port(o_spi_copi).signal(   "0000" )
        .port(o_rx_dv).signal(      "1100" )
        .port(o_rx_byte).signal(    {0xBB, 0} ).repeatEachStep(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedSendTrace + expectedReceiveTrace));
}

// TODO: parameterised MODE
//       -> via parameter to verilog module
// - does o_spi_clk idle to high or low?
// - is data set at rising/falling edge?
// - is data sampled at rising/falling edge?

// TODO: parameterised CLKS_PER_HALF_BIT
//        -> via parameter to verilog module