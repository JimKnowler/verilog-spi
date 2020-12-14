#pragma once

#include "gtest-verilog/TestBench.h"
#include "gtest-verilog/Trace.h"

#include "VSPIController.h"

/// @brief Expectation is that this would be generated from verilator output with python script

namespace spi_controller {
    extern PortDescription i_clk;
    extern PortDescription o_tx_ready;
    extern PortDescription o_spi_clk;
    extern PortDescription o_spi_copi;

    class TestBench : public ::TestBench<VSPIController> {
    public:
        virtual void onNextStep() override {
            Step step;
            
            step.port(i_clk) = core().i_clk;
            step.port(o_tx_ready) = core().o_tx_ready;
            step.port(o_spi_clk) = core().o_spi_clk;
            step.port(o_spi_copi) = core().o_spi_copi;

            trace.append(step);
        }

        Trace trace;
    };

}
