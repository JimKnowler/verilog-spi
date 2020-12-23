#pragma once

#include "spi/SPIControllerTestBench.h"
#include "spi/SPIPeripheralTestBench.h"

namespace testing {

    /// @class Helpers
    /// @brief Common Test helper functions used by multiple test fixtures
    class Helpers {
    public:
        static void controllerSetupSendByte(
            spicontrollertestbench::SPIControllerTestBench& testBench,
            uint8_t byte
        );
    
        static void controllerSimulateReceiveByte(
            spicontrollertestbench::SPIControllerTestBench& testBench,
            uint8_t byte, 
            uint32_t numStepsSetup = 0,
            uint32_t numStepsValid = 4,
            uint32_t numStepsPadding = 0
        );

        static void peripheralSetupSendByte(
            spiperipheraltestbench::SPIPeripheralTestBench& testBench,
            uint8_t byte
        );

        static void peripheralSimulateReceiveByte(
            spiperipheraltestbench::SPIPeripheralTestBench& testBench,
            uint8_t byte,
            uint32_t numStepsSetup = 0,
            uint32_t numStepsValid = 4,
            uint32_t numStepsPadding = 0
        );
    };
}

