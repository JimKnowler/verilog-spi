#include <gtest/gtest.h>

#include "Trace.h"

#include "SPIControllerPort.h"

TEST(Trace, ShouldConstruct) {
    Trace trace;

    ASSERT_EQ(trace.getSteps().size(), 0);
}

TEST(Trace, ShouldAddStep) {
    Trace trace;

    Step step;
    trace.append(step);

    ASSERT_EQ(trace.getSteps().size(), 1);
}

TEST(Trace, ShouldClear) {
    Trace trace;
    
    Step step;
    trace.append(step);
    
    trace.clear();
    ASSERT_EQ(trace.getSteps().size(), 0);
}

TEST(Trace, ShouldRegisterPorts) {
    Trace trace;
    //trace.registerPort()

    // todo: register Port ID, name, Verilator module offset, etc
}

// todo:
// - generic - only store ports that have data
// - equality (of ports that have data in both traces)
// - print to TTY
// - generate/configure ports from Verilator module