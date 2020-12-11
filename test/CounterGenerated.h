#pragma once

#include "PortDescription.h"

/// @brief Expectation is that this would be generated from verilator output with python script

/// @todo implement this
/// @todo consider that Counter & SPIController both have ports with the name 'i_clk' 

namespace counter {
    extern PortDescription i_clk;
    extern PortDescription i_reset;
    // todo extern PortDescription o_value;     // 16bits wide
}