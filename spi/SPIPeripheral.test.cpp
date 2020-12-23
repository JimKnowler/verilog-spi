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
            testBench.core().i_spi_cs_n = 1;

            testBench.reset();
            testBench.trace.clear();
        }

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

        void helperSimulateReceiveByte(uint8_t byte, uint32_t numStepsSetup = 0, uint32_t numStepsValid = 4, uint32_t numStepsPadding = 0) {
            auto& core = testBench.core();

            for (uint32_t index = 0; index < 8; index++) {
                uint32_t cumulativeSteps = 0;
                
                core.i_spi_copi = 0;

                for (uint32_t i=0; i<numStepsSetup; i++) {
                    core.i_spi_clk = (cumulativeSteps < 2);
                    testBench.nextStep();
                    cumulativeSteps += 1;
                }

                core.i_spi_copi = (byte >> (7-index)) & 0x1;                
                for (uint32_t i=0; i<numStepsValid; i++) {
                    core.i_spi_clk = (cumulativeSteps < 2);
                    testBench.nextStep();
                    cumulativeSteps += 1;
                }

                core.i_spi_copi = 0;
                
                for (uint32_t i=0; i<numStepsPadding; i++) {
                    core.i_spi_clk = (cumulativeSteps < 2);
                    testBench.nextStep();
                    cumulativeSteps += 1;
                }
            }
        }

        SPIPeripheralTestBench testBench;
    };
}

TEST_F(SPIPeripheral, ShouldConstructFixture) {
}

TEST_F(SPIPeripheral, ShouldReset) {
    testBench.reset();

    const Trace expectedTrace = TraceBuilder()
        .port(i_tx_dv).signal("0")
        .port(i_tx_byte).signal({0})
        .allPorts().repeat(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIPeripheral, ShouldSetupSend) {
    auto& core = testBench.core();

    core.i_tx_dv = 1;
    core.i_tx_byte = 0xAB;
    testBench.tick();

    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    testBench.tick();

    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal("1010")
        .port(i_tx_dv).signal("1100")
        .port(o_rx_dv).signal("0000")
        .port(o_rx_byte).signal({0,0,0,0})
        .port(o_spi_cipo).signal("0000")
        .port(i_tx_byte).signal({0xAB, 0}).repeatEachStep(2);

    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIPeripheral, ShouldSendByte0xFF) { 
    helperSetupSendByte(0xFF);
    
    testBench.core().i_spi_cs_n = 0;
    
    helperSimulateReceiveByte(0);
    
    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )         // should be 0 while sending
        .port(o_rx_byte).signal( {0,0,0,0})     // should be pulsed every other tick while sending
        .port(i_spi_clk).signal( "1100")        // alternate each fpga clock tick
        .port(o_spi_cipo).signal( "1111" )
        .port(i_spi_cs_n).signal( "0000" )
        .allPorts().repeat(8);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIPeripheral, ShouldSendByte0xAA) { 
    helperSetupSendByte(0xAA);                      // 0xAA => 0b10101010

    testBench.core().i_spi_cs_n = 0;
    
    helperSimulateReceiveByte(0);
    
    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )         // should be 0 while sending
        .port(o_rx_byte).signal( {0,0,0,0})     // should be pulsed every other tick while sending
        .port(i_spi_clk).signal( "1100")        // alternate each fpga clock tick
        .port(i_spi_cs_n).signal( "0000" )
        .allPorts().repeat(8)
        .port(o_spi_cipo).signal( "11110000" ).repeat(4);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIPeripheral, ShouldSendByte0x55) { 
    helperSetupSendByte(0x55);                      // 0x55 => 0b01010101       

    testBench.core().i_spi_cs_n = 0;

    helperSimulateReceiveByte(0);
    
    const Trace expectedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .allPorts().repeat(8)
        .port(o_spi_cipo).signal( "00001111" ).repeat(4);
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedTrace));
}

TEST_F(SPIPeripheral, ShouldReceiveByte0xAA) {
    helperSetupSendByte(0); 

    testBench.core().i_spi_cs_n = 0;
    
    helperSimulateReceiveByte(0xAA);            // 0xAA => 0b10101010

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )
        .allPorts().repeat(8)
        .port(i_spi_copi).signal( "11110000" ).repeat(4);


    // receive o_rx_dv & o_rx_byte
    testBench.tick(3);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "101010" )
        .port(o_rx_dv).signal( "001100" ) 
        .port(o_rx_byte).signal( {0,0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "000000")
        .port(i_spi_cs_n).signal( "000000" )
        .port(o_spi_cipo).signal( "000000" )
        .port(i_spi_copi).signal( "000000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldReceiveByte0x55) {
    helperSetupSendByte(0); 

    testBench.core().i_spi_cs_n = 0;
    
    helperSimulateReceiveByte(0x55);            // 0x55 => 0b01010101

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )
        .allPorts().repeat(8)
        .port(i_spi_copi).signal( "00001111" ).repeat(4);
    
    // receive o_rx_dv & o_rx_byte
    testBench.tick(3);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "101010" )
        .port(o_rx_dv).signal( "001100" ) 
        .port(o_rx_byte).signal( {0,0x55,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "000000")
        .port(i_spi_cs_n).signal( "000000" )
        .port(o_spi_cipo).signal( "000000" )
        .port(i_spi_copi).signal( "000000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldReceiveByteOnFallingEdge) {
    helperSetupSendByte(0); 

    testBench.core().i_spi_cs_n = 0;
    
    helperSimulateReceiveByte(0xAA, 1, 2, 1);

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )
        .allPorts().repeat(8)
        .port(i_spi_copi).signal( "01100000" ).repeat(4);


    // receive o_rx_dv & o_rx_byte
    testBench.tick(3);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "101010" )
        .port(o_rx_dv).signal( "001100" ) 
        .port(o_rx_byte).signal( {0,0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "000000")
        .port(i_spi_cs_n).signal( "000000" )
        .port(o_spi_cipo).signal( "000000" )
        .port(i_spi_copi).signal( "000000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldSend_0xAA_AndReceive_0x55) {
    helperSetupSendByte(0xAA); 

    testBench.core().i_spi_cs_n = 0;
    
    helperSimulateReceiveByte(0x55);

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .allPorts().repeat(8)
        .port(o_spi_cipo).signal( "11110000" ).repeat(4)
        .port(i_spi_copi).signal( "00001111" ).repeat(4);
    
    // receive o_rx_dv & o_rx_byte
    testBench.tick(3);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "101010" )
        .port(o_rx_dv).signal( "001100" ) 
        .port(o_rx_byte).signal( {0,0x55,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "000000")
        .port(i_spi_cs_n).signal( "000000" )
        .port(o_spi_cipo).signal( "000000" )
        .port(i_spi_copi).signal( "000000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldSend_0x55_AndReceive_0xAA) {
    helperSetupSendByte(0x55); 

    testBench.core().i_spi_cs_n = 0;
    
    helperSimulateReceiveByte(0xAA);

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .allPorts().repeat(8)
        .port(o_spi_cipo).signal( "00001111" ).repeat(4)
        .port(i_spi_copi).signal( "11110000" ).repeat(4);
    
    // receive o_rx_dv & o_rx_byte
    testBench.tick(3);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "101010" )
        .port(o_rx_dv).signal( "001100" ) 
        .port(o_rx_byte).signal( {0,0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "000000")
        .port(i_spi_cs_n).signal( "000000" )
        .port(o_spi_cipo).signal( "111111" )    // note: signal is hi because last bit of 0xAA is hi
        .port(i_spi_copi).signal( "000000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldSendAndReceiveMultipleBytes) {
    // simulate sending first byte
    helperSetupSendByte(0x12);    
    testBench.core().i_spi_cs_n = 0;                  
    helperSimulateReceiveByte(0x34);
    testBench.tick(3);
    testBench.trace.clear();

    // simulate sending second byte
    helperSetupSendByte(0x55);
    helperSimulateReceiveByte(0xAA);

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .allPorts().repeat(8)
        .port(o_spi_cipo).signal( "00001111" ).repeat(4)
        .port(i_spi_copi).signal( "11110000" ).repeat(4);
    
    // receive o_rx_dv & o_rx_byte
    testBench.tick(3);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "101010" )
        .port(o_rx_dv).signal( "001100" ) 
        .port(o_rx_byte).signal( {0,0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "000000")
        .port(i_spi_cs_n).signal( "000000" )
        .port(o_spi_cipo).signal( "111111" )    // note: signal is hi because last bit of 0xAA is hi
        .port(i_spi_copi).signal( "000000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldObserveChipSelect) {
    // deactivate chip select (inactive high)
    testBench.core().i_spi_cs_n = 1;

    // simulate sending second byte
    helperSetupSendByte(0x55);
    helperSimulateReceiveByte(0xAA);

    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "1111" )
        .port(o_spi_cipo).signal( "0000" )
        .allPorts().repeat(8)
        .port(i_spi_copi).signal( "11110000" ).repeat(4);
        
    // receive o_rx_dv & o_rx_byte
    testBench.tick(3);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "101010" )
        .port(o_rx_dv).signal( "000000" ) 
        .port(o_rx_byte).signal( {0,0,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "000000")
        .port(i_spi_cs_n).signal( "111111" )
        .port(o_spi_cipo).signal( "000000" )
        .port(i_spi_copi).signal( "000000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

// todo: signal immediately on CIPO when CS goes low?

// TODO: parameterised MODE
//       -> via parameter to verilog module
// - does o_spi_clk idle while high or low?
// - is data set at rising/falling edge?
// - is data sampled at rising/falling edge?

// TODO: parameterised CLKS_PER_HALF_BIT
//        -> via parameter to verilog module