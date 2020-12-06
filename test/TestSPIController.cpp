#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "VSPIController.h"
#include "TestBench.h"

#include "Trace.h"

using namespace testing;

#define MatchRisingEdge(x) Field(&Trace::Tick::risingEdge, x)
#define MatchFallingEdge(x) Field(&Trace::Tick::fallingEdge, x)
#define MatchTxReady(x) Field(&Trace::Signals::o_tx_ready, x)
#define MatchSpiClk(x) Field(&Trace::Signals::o_spi_clk, x)
#define MatchSpiCOPI(x) Field(&Trace::Signals::o_spi_copi, x)

namespace {
    class SPIControllerTest : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();

            trace.clear();
        }

        void tick(uint64_t numTicks = 1) {
            for (uint64_t i=0; i<numTicks; i++) {
                Trace::Tick tick;
                tick.index = trace.tickCount++;

                auto& core = testBench.core();

                testBench.clockRisingEdge();
                tick.risingEdge.probe(core);

                testBench.clockFallingEdge();
                tick.fallingEdge.probe(core);

                trace.ticks.emplace_back(tick);
            }
        }
        
        TestBench<VSPIController> testBench;

        Trace trace;
    };
}

TEST_F(SPIControllerTest, ShouldConstructFixture) {
}

TEST_F(SPIControllerTest, ShouldReportReadyToTransmit) {
    ASSERT_EQ(1, testBench.core().o_tx_ready);
}

// should not change clock while not sending
TEST_F(SPIControllerTest, ShouldIdleSpiClockWhileIdle) {
    tick(50);

    EXPECT_THAT(trace.ticks, Each(MatchRisingEdge(MatchSpiClk(Eq(0)))));
    EXPECT_THAT(trace.ticks, Each(MatchFallingEdge(MatchSpiClk(Eq(0)))));
}

TEST_F(SPIControllerTest, ShouldSendByteF) { 
    auto& core = testBench.core();
    
    // start sending 0xf by pulsing command lines
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xf;
    tick();
    
    // reset trace, so we only capture signals during the transmission
    trace.clear();

    // send in progress
    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    tick((8*2));

    // o_tx_ready should be 0 while sending
    EXPECT_THAT(trace.ticks, Each(MatchRisingEdge(MatchTxReady(Eq(0)))));
    EXPECT_THAT(trace.ticks, Each(MatchFallingEdge(MatchTxReady(Eq(0)))));

    // spi clk should be pulsed while sending
    std::vector<Trace::Tick> traceEven;
    std::copy_if(trace.ticks.begin(), trace.ticks.end(), std::back_inserter(traceEven), [](Trace::Tick& tick){ return tick.index % 2 == 0; });
    EXPECT_THAT(traceEven, Each(MatchRisingEdge(MatchSpiClk(Eq(1)))));
    EXPECT_THAT(traceEven, Each(MatchFallingEdge(MatchSpiClk(Eq(1)))));

    std::vector<Trace::Tick> traceOdd;
    std::copy_if(trace.ticks.begin(), trace.ticks.end(), std::back_inserter(traceOdd), [](Trace::Tick& tick){ return tick.index % 2 == 1; });
    EXPECT_THAT(traceOdd, Each(MatchRisingEdge(MatchSpiClk(Eq(0)))));
    EXPECT_THAT(traceOdd, Each(MatchFallingEdge(MatchSpiClk(Eq(0)))));

    // all bits should be sent as 1
    EXPECT_THAT(trace.ticks, Each(MatchRisingEdge(MatchSpiCOPI(Eq(1)))));
    EXPECT_THAT(trace.ticks, Each(MatchRisingEdge(MatchSpiCOPI(Eq(1)))));
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