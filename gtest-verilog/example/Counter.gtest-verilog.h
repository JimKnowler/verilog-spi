#pragma once

#include "gtest-verilog/TestBench.h"
#include "gtest-verilog/Trace.h"

#include "VCounter.h"

/// @brief Expectation is that this would be generated from verilator output with python script

namespace counter {
    extern ::testing_verilog::PortDescription i_clk;
    extern ::testing_verilog::PortDescription i_reset;
    extern ::testing_verilog::PortDescription o_value;

    class TestBench : public ::testing_verilog::TestBench<VCounter> {
    public:
        virtual void onNextStep() override;

        ::testing_verilog::Trace trace;
    };
}