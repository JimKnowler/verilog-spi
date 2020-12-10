#include <gtest/gtest.h>

#include "Trace.h"

#include "SPIControllerGenerated.h"

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

TEST(Trace, ShouldGetPortMaskForEmptyTrace) {
    const Trace trace;
    
    ASSERT_EQ(0, trace.getPortMask());
}

TEST(Trace, ShouldGetPortMaskForNonEmptyTrace) {
    Step step;
    step.port(0) = 1;
    step.port(3) = 0;

    Trace trace;
    trace.append(step);

    // bits 0 and 3 should be set in mask
    // bits 0 = 1, bit 3 = 8
    ASSERT_EQ(0x00000009, trace.getPortMask());
}


// todo:
// - print to TTY
// - generate/configure ports from Verilator module - python script?
