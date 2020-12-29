#include "Helpers.h"

namespace testing {

    void Helpers::controllerSetupSendByte(
        spicontrollertestbench::SPIControllerTestBench& testBench,
        uint8_t byte
    ) {
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

    void Helpers::controllerSimulateReceiveByte(
        spicontrollertestbench::SPIControllerTestBench& testBench,
        uint8_t byte, 
        uint32_t numStepsSetup,
        uint32_t numStepsValid,
        uint32_t numStepsPadding
    ) {
        auto& core = testBench.core();
        
        for (uint32_t index = 0; index < 8; index++) {
            core.i_spi_cipo = 0;
            for (uint32_t i=0; i<numStepsSetup; i++) {
                testBench.step();
            }

            core.i_spi_cipo = (byte >> (7-index)) & 0x1;                
            for (uint32_t i=0; i<numStepsValid; i++) {
                testBench.step();
            }

            core.i_spi_cipo = 0;
            
            for (uint32_t i=0; i<numStepsPadding; i++) {
                testBench.step();
            }
        }
    }

    void Helpers::peripheralSetupSendByte(
        spiperipheraltestbench::SPIPeripheralTestBench& testBench,
        uint8_t byte
    ) {
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

    void Helpers::peripheralSimulateReceiveByte(
        spiperipheraltestbench::SPIPeripheralTestBench& testBench,
        uint8_t byte,
        uint32_t numStepsSetup,
        uint32_t numStepsValid,
        uint32_t numStepsPadding
    ) {
        auto& core = testBench.core();

        for (uint32_t index = 0; index < 8; index++) {
            uint32_t cumulativeSteps = 0;
            
            core.i_spi_copi = 0;

            for (uint32_t i=0; i<numStepsSetup; i++) {
                core.i_spi_clk = (cumulativeSteps < 2);
                testBench.step();
                cumulativeSteps += 1;
            }

            core.i_spi_copi = (byte >> (7-index)) & 0x1;                
            for (uint32_t i=0; i<numStepsValid; i++) {
                core.i_spi_clk = (cumulativeSteps < 2);
                testBench.step();
                cumulativeSteps += 1;
            }

            core.i_spi_copi = 0;
            
            for (uint32_t i=0; i<numStepsPadding; i++) {
                core.i_spi_clk = (cumulativeSteps < 2);
                testBench.step();
                cumulativeSteps += 1;
            }
        }
    }
}
