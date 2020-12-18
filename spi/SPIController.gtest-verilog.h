#pragma once

#include "gtest-verilog/TestBench.h"
#include "gtest-verilog/Trace.h"

#include "VSPIController.h"

/// @brief Expectation is that this would be generated from verilator output with python script

namespace spi_controller {
    extern ::testing_verilog::PortDescription i_clk;
    extern ::testing_verilog::PortDescription o_tx_ready;
    extern ::testing_verilog::PortDescription o_spi_clk;
    extern ::testing_verilog::PortDescription o_spi_copi;

    class TestBench : public ::testing_verilog::TestBench<VSPIController> {
    public:
        virtual void onNextStep() override;

        ::testing_verilog::Trace trace;
    };

}
