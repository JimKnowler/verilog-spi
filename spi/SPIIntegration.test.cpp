#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "spi/SPIPeripheralTestBench.h"
using namespace spiperipheraltestbench;

#include "spi/SPIControllerTestBench.h"
using namespace spicontrollertestbench;

namespace {
    class SPIIntegration : public ::testing::Test {
    public:
        void SetUp() override {
            testBenchPeripheral.core().i_spi_cs_n = 1;

            testBenchPeripheral.reset();
            testBenchPeripheral.trace.clear();

            testBenchController.reset();
            testBenchController.trace.clear();
        }

        void helperControllerSetupSendByte(uint8_t byte) {
            auto& core = testBenchController.core();
            
            // start sending by pulsing command lines
            core.i_tx_dv = 1;
            core.i_tx_byte = byte;
            testBenchController.tick();
            
            // reset trace, so we only capture signals during the transmission
            testBenchController.trace.clear();

            // send in progress, clear the external inputs
            core.i_tx_dv = 0;
            core.i_tx_byte = 0;
        }

        void helperPeripheralSetupSendByte(uint8_t byte) {
            auto& core = testBenchPeripheral.core();
            
            // start sending by pulsing command lines
            core.i_tx_dv = 1;
            core.i_tx_byte = byte;
            testBenchPeripheral.tick();
            
            // reset trace, so we only capture signals during the transmission
            testBenchPeripheral.trace.clear();

            // send in progress, clear the external inputs
            core.i_tx_dv = 0;
            core.i_tx_byte = 0;
        }

        void tick(uint32_t numTicks) {
            for (uint32_t i=0; i<numTicks; i++) {
                auto& controller = testBenchController.core();
                auto& peripheral = testBenchPeripheral.core();

                for (uint32_t j=0; j<2; j++) {
                    peripheral.i_spi_clk = controller.o_spi_clk;
                    peripheral.i_spi_copi = controller.o_spi_copi;
                    controller.i_spi_cipo = peripheral.o_spi_cipo;

                    testBenchController.nextStep();
                    testBenchPeripheral.nextStep();
                }
            }
        }

        SPIControllerTestBench testBenchController;
        SPIPeripheralTestBench testBenchPeripheral;
    };
}

TEST_F(SPIIntegration, ShouldConstructFixture) {

}

TEST_F(SPIIntegration, ShouldSendAndReceiveOneByte) {
    helperPeripheralSetupSendByte(0xAA);
    helperControllerSetupSendByte(0x55);

    // activate chip select
    testBenchPeripheral.core().i_spi_cs_n = 0;

    // simulate the communication
    tick(8 * 2);

    // check controller received data
    tick(1);
    EXPECT_EQ(testBenchController.core().o_rx_byte, 0xAA);

    // check peripheral received data
    tick(2);
    EXPECT_EQ(testBenchPeripheral.core().o_rx_byte, 0x55);

    // debugging
    printf("debugging info\n");
    tick(1);
    printf("controller\n");
    std::cout << testBenchController.trace;
    printf("peripheral\n");
    std::cout << testBenchPeripheral.trace;
}

#if 0
TEST_F(SPIIntegration, ShouldSendAndReceiveMultipleBytes) {

}
#endif
