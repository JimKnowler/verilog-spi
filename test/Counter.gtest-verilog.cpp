#include "Counter.gtest-verilog.h"

namespace counter {
    PORT_DESCRIPTION(0, i_clk);
    PORT_DESCRIPTION(1, i_reset);
    // todo: o_value

    void TestBench::onNextStep() {
        Step step;
        
        step.port(i_clk) = core().i_clk;
        step.port(i_reset) = core().i_reset;

        // todo: 16bits wide
        //step.port(o_value) = core().o_value;

        trace.append(step);
    }
}