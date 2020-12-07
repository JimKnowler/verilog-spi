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

TEST(Trace, ShouldFailToAddStepsWithDifferentPorts) {
    Trace trace;
    
    Step step1;
    trace.append(step1);

    Step step2;
    step2.port(1) = false;
    ASSERT_ANY_THROW(trace.append(step2));
}

TEST(Trace, ShouldRegisterPorts) {
    Trace trace;

    // todo: register Port ID, name, Verilator module offset, etc
}

// todo:
// - equality (of ports that have data in both traces)
// - print to TTY
// - generate/configure ports from Verilator module
