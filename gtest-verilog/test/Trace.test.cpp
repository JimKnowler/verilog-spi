#include <gtest/gtest.h>

#include "gtest-verilog/Trace.h"

namespace {
    PORT_DESCRIPTION(0, test_port_0, 1);
    PORT_DESCRIPTION(3, test_port_3, 1);
}

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
    step2.port(test_port_3) = false;
    ASSERT_ANY_THROW(trace.append(step2));
}

TEST(Trace, ShouldGetPortMaskForEmptyTrace) {
    const Trace trace;
    
    ASSERT_EQ(0, trace.getPortMask());
}

TEST(Trace, ShouldGetPortMaskForNonEmptyTrace) {
    Step step;
    step.port(test_port_0) = 1;
    step.port(test_port_3) = 0;

    Trace trace;
    trace.append(step);

    // bits 0 and 3 should be set in mask
    // bits 0 = 1, bit 3 = 8
    ASSERT_EQ(0x00000009, trace.getPortMask());
}

TEST(Trace, ShouldGetPortDescription) {
    Step step;
    step.port(test_port_0) = 1;
    step.port(test_port_3) = 0;

    Trace trace;
    trace.append(step);

    EXPECT_EQ(&test_port_0, &trace.getPortDescription(0));
    EXPECT_EQ(&test_port_3, &trace.getPortDescription(3));
}

TEST(Trace, ShouldFailToGetPortDescriptionForUnknownPort) {
    Step step;
    step.port(test_port_0) = 1;
    step.port(test_port_3) = 0;

    Trace trace;
    trace.append(step);

    ASSERT_ANY_THROW(trace.getPortDescription(1));
    ASSERT_ANY_THROW(trace.getPortDescription(2));
    ASSERT_ANY_THROW(trace.getPortDescription(4));
}

TEST(Trace, ShouldFailToGetPortDescriptionForEmptyTrace) {
    Trace trace;

    ASSERT_ANY_THROW(trace.getPortDescription(0)); 
    ASSERT_ANY_THROW(trace.getPortDescription(1));
    ASSERT_ANY_THROW(trace.getPortDescription(2));
    ASSERT_ANY_THROW(trace.getPortDescription(3)); 
    ASSERT_ANY_THROW(trace.getPortDescription(4));
}


// todo:
// - print to TTY
// - generate/configure ports from Verilator module - python script?
