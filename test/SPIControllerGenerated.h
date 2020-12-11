#pragma once

#include "PortDescription.h"

/// @brief Expectation is that this would be generated from verilator output with python script

namespace spi_controller {
    extern PortDescription i_clk;
    extern PortDescription o_tx_ready;
    extern PortDescription o_spi_clk;
    extern PortDescription o_spi_copi;
}