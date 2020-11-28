#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "VSPIController.h"
#include "TestBench.h"

using namespace testing;

namespace {
    class SPIControllerTest : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();
        }

        void tick(uint64_t numTicks = 1) {
            for (uint64_t i=0; i<numTicks; i++) {
                Tick tick;

                auto& core = testBench.core();

                testBench.clockRisingEdge();
                tick.risingEdge.o_tx_ready = core.o_tx_ready;

                testBench.clockFallingEdge();
                tick.fallingEdge.o_tx_ready = core.o_tx_ready;

                trace.emplace_back(tick);
            }
        }

        struct Signals {
            uint8_t o_tx_ready;
        };

        struct Tick {
            Signals risingEdge;
            Signals fallingEdge;
        };

        std::vector<Tick> trace;
        TestBench<VSPIController> testBench;
    };
}

TEST_F(SPIControllerTest, ShouldConstruct) {
}

TEST_F(SPIControllerTest, ShouldReportReadyToTransmit) {
    ASSERT_EQ(1, testBench.core().o_tx_ready);
}

TEST_F(SPIControllerTest, ShouldSendByte) { 
    auto& core = testBench.core();
    
    // start sending 0xf
    core.i_tx_dv = 1;
    core.i_tx_byte = 0xf;
    tick();

    // send should be initiated so we can drop send signals
    core.i_tx_dv = 0;
    core.i_tx_byte = 0;
    tick((8*2) - 1);

    EXPECT_THAT(trace, Each(Field(&Tick::risingEdge, Field(&Signals::o_tx_ready, Eq(0)))));
    EXPECT_THAT(trace, Each(Field(&Tick::fallingEdge, Field(&Signals::o_tx_ready, Eq(0)))));
}

// send and receive byte - when is o_RX_DV pulsed? when does o_TX_Ready go high?
// 

// note: assume single clock cycle per half tick
// note: helper to tick + sample outputs at rising/falling edge

// send TX byte
// cache TX byte when starting to write
//  



// TODO - parameterised CLKS_PER_HALF_BIT
// TODO - parameterised SPI mode