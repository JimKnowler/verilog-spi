#pragma once

#include "gtest-verilog/TestBench.h"
#include "gtest-verilog/Trace.h"

#include "VCounter.h"

/// @brief Expectation is that this would be generated from verilator output with python script

namespace counter {
    extern PortDescription i_clk;
    extern PortDescription i_reset;
    /// @todo extern PortDescription o_value;     // 16bits wide}

    class TestBench : public ::TestBench<VCounter> {
    public:
        virtual void onNextStep() override;

        Trace trace;
    };
}