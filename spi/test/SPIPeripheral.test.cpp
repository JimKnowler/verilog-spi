#include <vector>

#include <gmock/gmock.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "spi/SPIPeripheralTestBench.h"
using namespace spiperipheraltestbench;

#include "Helpers.h"

namespace {
    class SPIPeripheral : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.core().i_spi_cs_n = 1;

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
    Helpers::peripheralSetupSendByte(testBench, 0xFF);
    
    testBench.core().i_spi_cs_n = 0;
    
    Helpers::peripheralSimulateReceiveByte(testBench, 0);
    
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
    Helpers::peripheralSetupSendByte(testBench, 0xAA);                      // 0xAA => 0b10101010

    testBench.core().i_spi_cs_n = 0;
    
    Helpers::peripheralSimulateReceiveByte(testBench, 0);
    
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
    Helpers::peripheralSetupSendByte(testBench, 0x55);

    testBench.core().i_spi_cs_n = 0;

    Helpers::peripheralSimulateReceiveByte(testBench, 0);
    
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
    Helpers::peripheralSetupSendByte(testBench, 0); 

    testBench.core().i_spi_cs_n = 0;
    
    Helpers::peripheralSimulateReceiveByte(testBench, 0xAA);

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
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldReceiveByte0x55) {
    Helpers::peripheralSetupSendByte(testBench, 0); 

    testBench.core().i_spi_cs_n = 0;
    
    Helpers::peripheralSimulateReceiveByte(testBench, 0x55);            // 0x55 => 0b01010101

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
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0x55,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldReceiveByteOnFallingEdge) {
    Helpers::peripheralSetupSendByte(testBench, 0); 

    testBench.core().i_spi_cs_n = 0;
    
    Helpers::peripheralSimulateReceiveByte(testBench, 0xAA, 8, 1, 2, 1);

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
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldSend_0xAA_AndReceive_0x55) {
    Helpers::peripheralSetupSendByte(testBench, 0xAA); 

    testBench.core().i_spi_cs_n = 0;
    
    Helpers::peripheralSimulateReceiveByte(testBench, 0x55);

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
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0x55,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldSend_0x55_AndReceive_0xAA) {
    Helpers::peripheralSetupSendByte(testBench, 0x55); 

    testBench.core().i_spi_cs_n = 0;
    
    Helpers::peripheralSimulateReceiveByte(testBench, 0xAA);

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
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "1111" )    // note: signal is hi because last bit of 0xAA is hi
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldSendAndReceiveMultipleBytes) {
    // simulate sending first byte
    Helpers::peripheralSetupSendByte(testBench, 0x12);    
    testBench.core().i_spi_cs_n = 0;                  
    Helpers::peripheralSimulateReceiveByte(testBench, 0x34);
    testBench.tick(3);
    testBench.trace.clear();

    // simulate sending second byte
    Helpers::peripheralSetupSendByte(testBench, 0x55);
    Helpers::peripheralSimulateReceiveByte(testBench, 0xAA);

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
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "1111" )    // note: signal is hi because last bit of 0x55 is hi
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldObserveChipSelect) {
    // deactivate chip select (inactive high)
    testBench.core().i_spi_cs_n = 1;

    // simulate sending second byte
    Helpers::peripheralSetupSendByte(testBench, 0x55);
    Helpers::peripheralSimulateReceiveByte(testBench, 0xAA);

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

TEST_F(SPIPeripheral, ShouldResetWhenChipSelectGoesHigh) {
    // receive and send part of a byte
    testBench.core().i_spi_cs_n = 0;
    Helpers::peripheralSetupSendByte(testBench, 0x12);    
    Helpers::peripheralSimulateReceiveByte(testBench, 0x34, 4);
    
    // chip select goes high, forcing reset
    testBench.core().i_spi_cs_n = 1;   
    testBench.tick();
    testBench.trace.clear();

    // chip select goes low
    testBench.core().i_spi_cs_n = 0;
    Helpers::peripheralSetupSendByte(testBench, 0x55);             
    Helpers::peripheralSimulateReceiveByte(testBench, 0xAA);
    
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
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "1111" )    // note: signal is hi because last bit of 0x55 is hi
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}

TEST_F(SPIPeripheral, ShouldClearTXBufferAfterSendIsComplete) {
    // receive and send byte
    testBench.core().i_spi_cs_n = 0;
    Helpers::peripheralSetupSendByte(testBench, 0x12);    
    Helpers::peripheralSimulateReceiveByte(testBench, 0x34);
    
    testBench.tick();
    testBench.trace.clear();

    // receive a byte (without setting up a send)
    Helpers::peripheralSimulateReceiveByte(testBench, 0xAA);
    
    const Trace expectedReceiveTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "0000" )
        .port(o_rx_byte).signal( {0,0,0,0})
        .port(i_spi_clk).signal( "1100")
        .port(i_spi_cs_n).signal( "0000" )
        .allPorts().repeat(8)
        .port(o_spi_cipo).signal( "00000000" ).repeat(4)
        .port(i_spi_copi).signal( "11110000" ).repeat(4);
    
    // receive o_rx_dv & o_rx_byte
    testBench.tick(2);
    
    const Trace expectedReceivedTrace = TraceBuilder()
        .port(i_clk).signal( "1010" )
        .port(o_rx_dv).signal( "1100" ) 
        .port(o_rx_byte).signal( {0xAA,0}).repeatEachStep(2)
        .port(i_spi_clk).signal( "0000")
        .port(i_spi_cs_n).signal( "0000" )
        .port(o_spi_cipo).signal( "0000" )                      // should be 0, because we didn't setup a send byte
        .port(i_spi_copi).signal( "0000" );
    
    EXPECT_THAT(testBench.trace, MatchesTrace(expectedReceiveTrace + expectedReceivedTrace));
}


// TODO: parameterised MODE
//       -> via parameter to verilog module
// - does o_spi_clk idle while high or low?
// - is data set at rising/falling edge?
// - is data sampled at rising/falling edge?

// TODO: parameterised CLKS_PER_HALF_BIT
//        -> via parameter to verilog module