#include "SPIController.gtest-verilog.h"

/// @brief Expectation is that this would be generated from verilator output with python script

namespace spi_controller {
    PORT_DESCRIPTION(0, i_clk, 1);
    PORT_DESCRIPTION(1, o_tx_ready, 1);
    PORT_DESCRIPTION(2, o_spi_clk, 1);
    PORT_DESCRIPTION(3, o_spi_copi, 1);

    void TestBench::onNextStep() {
        Step step;
        
        step.port(i_clk) = (1 == core().i_clk);
        step.port(o_tx_ready) = (1 == core().o_tx_ready);
        step.port(o_spi_clk) = (1 == core().o_spi_clk);
        step.port(o_spi_copi) = (1 == core().o_spi_copi);

        trace.append(step);
    }
}