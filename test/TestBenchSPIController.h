#pragma once

#include "gtest-verilog/TestBench.h"
#include "gtest-verilog/Trace.h"

#include "VSPIController.h"

#include "SPIControllerGenerated.h"

// @strategy - put all of the VSPIController specific code here
//             that we could generate later with Python script..

class TestBenchSPIController : public TestBench<VSPIController> {
public:
    virtual void onNextStep() override {
        Step step;

        using namespace spi_controller;
        
        step.port(i_clk) = core().i_clk;
        step.port(o_tx_ready) = core().o_tx_ready;
        step.port(o_spi_clk) = core().o_spi_clk;
        step.port(o_spi_copi) = core().o_spi_copi;

        trace.append(step);
    }

    Trace trace;
};
