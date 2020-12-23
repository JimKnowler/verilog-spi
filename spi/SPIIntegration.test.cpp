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

#include "Helpers.h"

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
    Helpers::peripheralSetupSendByte(testBenchPeripheral, 0xAA);
    Helpers::controllerSetupSendByte(testBenchController, 0x55);

    // activate chip select
    testBenchPeripheral.core().i_spi_cs_n = 0;

    // simulate the communication
    const uint32_t kNumClksPerSpiClk = 2;               // todo: test with controllers with different PARAM values for SPI CLK length
    tick(8 * kNumClksPerSpiClk);

    // check controller received data
    tick(1);
    EXPECT_EQ(testBenchController.core().o_rx_byte, 0xAA);

    // check peripheral received data
    tick(1);
    EXPECT_EQ(testBenchPeripheral.core().o_rx_byte, 0x55);
}

#if 0
TEST_F(SPIIntegration, ShouldSendAndReceiveMultipleBytes) {

}
#endif
