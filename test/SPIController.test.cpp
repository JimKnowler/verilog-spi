#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "VSPIController.h"
#include "TestBench.h"

#include "Trace.h"

using namespace testing;

namespace {
    class SPIController : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();

            trace.clear();
        }

        // todo: move this into TestBench / Trace?
        //       or work with TestBench via callback after each step
        void tick(uint64_t numTicks = 1) {
            for (uint64_t i=0; i<numTicks; i++) {
                Trace::Step step;
                auto& core = testBench.core();

                testBench.step();
                step.probe(core);
                trace.append(step);

                testBench.step();
                step.probe(core);
                trace.append(step);
            }
        }
        
        TestBench<VSPIController> testBench;

        // todo: move this to TestBench?   
        //     or ask TestBench to work with Trace, via callback?
        //     share type templating?
        Trace trace;
    };
}

TEST_F(SPIController, ShouldConstructFixture) {
}

TEST_F(SPIController, ShouldReportReadyToTransmit) {
    ASSERT_EQ(1, testBench.core().o_tx_ready);
}

// should not change clock while not sending
TEST_F(SPIController, ShouldIdleSpiClockWhileIdle) {
    tick(50);

    // TODO - implement support for this :)
    /*
    Trace expected = TraceBuilder()
        .o_spi_clk().signal("00").repeat(50);

    EXPECT_THAT(trace, MatchesTrace(expected));
    */
}

TEST_F(SPIController, ShouldSendByteF) { 
    auto& core = testBench.core();
    
    // start sending 0xf by pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xf;
    tick();
    
    // reset trace, so we only capture signals during the transmission
    // TODO: or, should we include the first tick, to make sure that
    //       SPI lines are idle?
    trace.clear();

    // send in progress
    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    tick(8 * 2);

    // o_tx_ready should be 0 while sending
    // o_spi_clk should be pulsed every other tick while sending
    // o_spi_copi should be 1 while sending
    
    // TODO - implement support for this :)
    /*
    Trace expectedTrace = TraceBuilder()
             .i_clk("1010")
        .o_tx_ready("0000")
         .o_spi_clk("1100")
        .o_spi_copi("1111")
        .trace().repeat(8);
    
    EXPECT_THAT(trace, MatchesTrace(expectedTrace));
    */
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