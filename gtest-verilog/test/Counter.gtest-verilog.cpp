#include "Counter.gtest-verilog.h"

namespace counter {
    PORT_DESCRIPTION(0, i_clk, 1);
    PORT_DESCRIPTION(1, i_reset, 1);
    PORT_DESCRIPTION(2, o_value, 16);

    void TestBench::onNextStep() {
        Step step;
        
        step.port(i_clk) = (core().i_clk == 1);
        step.port(i_reset) = (core().i_reset == 1);
        //step.port(o_value) = core().o_value;

        trace.append(step);
    }
}