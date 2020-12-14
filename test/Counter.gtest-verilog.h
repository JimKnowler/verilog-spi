#pragma once

#include "gtest-verilog/TestBench.h"
#include "gtest-verilog/Trace.h"

#include "VCounter.h"

/// @brief Expectation is that this would be generated from verilator output with python script

/// @todo implement this
/// @todo consider that Counter & SPIController both have ports with the name 'i_clk' 

namespace counter {
    extern PortDescription i_clk;
    extern PortDescription i_reset;
    // todo extern PortDescription o_value;     // 16bits wide}

    class TestBench : public ::TestBench<VCounter> {
    public:
        /// @todo move to CPP file
        virtual void onNextStep() override {
            Step step;
            
            step.port(i_clk) = core().i_clk;
            step.port(i_reset) = core().i_reset;

            // todo: 16bits wide
            //step.port(o_value) = core().o_value;

            trace.append(step);
        }

        Trace trace;
    };
}