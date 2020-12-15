#include <gtest/gtest.h>

#include "gtest-verilog/TraceBuilder.h"

namespace {
    PORT_DESCRIPTION(0, test_port_0, 1);
    PORT_DESCRIPTION(1, test_port_1, 1);
    PORT_DESCRIPTION(2, test_port_2, 1);
    PORT_DESCRIPTION(3, test_port_3, 1);
}

TEST(TraceBuilder, ShouldConstruct) {
    TraceBuilder builder;
}

TEST(TraceBuilder, ShouldConvertToEmptyTrace) {
    Trace trace = TraceBuilder();

    ASSERT_EQ(trace.getSteps().size(), 0);
}

TEST(TraceBuilder, ShouldAddPort) {
    Trace trace = TraceBuilder()
        .port(test_port_0);

    ASSERT_EQ(trace.getSteps().size(), 0);

    // todo: check flag on Trace for which ports have been set
}

TEST(TraceBuilder, ShouldAddPorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0)
        .port(test_port_1)
        .port(test_port_2)
        .port(test_port_3);

    ASSERT_EQ(trace.getSteps().size(), 0);

    // todo: check flag on Trace for which ports have been set
}

TEST(TraceBuilder, ShouldAddSignalToPortWithBinarySyntax) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10110111");

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(test_port_0), 1);
    ASSERT_EQ(steps[1].port(test_port_0), 0);
    ASSERT_EQ(steps[2].port(test_port_0), 1);
    ASSERT_EQ(steps[3].port(test_port_0), 1);
    ASSERT_EQ(steps[4].port(test_port_0), 0);
    ASSERT_EQ(steps[5].port(test_port_0), 1);
    ASSERT_EQ(steps[6].port(test_port_0), 1);
    ASSERT_EQ(steps[7].port(test_port_0), 1);
}

TEST(TraceBuilder, ShouldAddSignalToPortWithHiLoSyntax) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("___-__-_");

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(test_port_0), 0);
    ASSERT_EQ(steps[1].port(test_port_0), 0);
    ASSERT_EQ(steps[2].port(test_port_0), 0);
    ASSERT_EQ(steps[3].port(test_port_0), 1);
    ASSERT_EQ(steps[4].port(test_port_0), 0);
    ASSERT_EQ(steps[5].port(test_port_0), 0);
    ASSERT_EQ(steps[6].port(test_port_0), 1);
    ASSERT_EQ(steps[7].port(test_port_0), 0);
}

TEST(TraceBuilder, ShouldFailToAddSignalWhenNoPortIsSelected) {
    ASSERT_ANY_THROW(TraceBuilder().signal("1"));
}

TEST(TraceBuilder, ShouldFailToAddSignalWithUnknownCharacter) {
    ASSERT_ANY_THROW(TraceBuilder().port(test_port_0).signal("!"));
}

TEST(TraceBuilder, ShouldAddMultipleSignalsToPort) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10110").signal("111");

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(test_port_0), 1);
    ASSERT_EQ(steps[1].port(test_port_0), 0);
    ASSERT_EQ(steps[2].port(test_port_0), 1);
    ASSERT_EQ(steps[3].port(test_port_0), 1);
    ASSERT_EQ(steps[4].port(test_port_0), 0);
    ASSERT_EQ(steps[5].port(test_port_0), 1);
    ASSERT_EQ(steps[6].port(test_port_0), 1);
    ASSERT_EQ(steps[7].port(test_port_0), 1);
}

TEST(TraceBuilder, ShouldAddSignalsToMultiplePorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("__")
        .port(test_port_1).signal("_-")
        .port(test_port_2).signal("-_")
        .port(test_port_3).signal("--");
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 2);
    ASSERT_EQ(steps[0].port(test_port_0), 0);
    ASSERT_EQ(steps[1].port(test_port_0), 0);
    ASSERT_EQ(steps[0].port(test_port_1), 0);
    ASSERT_EQ(steps[1].port(test_port_1), 1);
    ASSERT_EQ(steps[0].port(test_port_2), 1);
    ASSERT_EQ(steps[1].port(test_port_2), 0);
    ASSERT_EQ(steps[0].port(test_port_3), 1);
    ASSERT_EQ(steps[1].port(test_port_3), 1);
}

TEST(TraceBuilder, ShouldFailToGenerateTraceWhenPortsHaveSignalsWithDifferentLengths) {
    Trace trace;
    ASSERT_ANY_THROW(trace = TraceBuilder()
             .port(test_port_0).signal("__")
        .port(test_port_1).signal("_")
    );
}

TEST(TraceBuilder, ShouldRepeatSignal) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10").repeat(4);

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(test_port_0), 1);
    ASSERT_EQ(steps[1].port(test_port_0), 0);
    ASSERT_EQ(steps[2].port(test_port_0), 1);
    ASSERT_EQ(steps[3].port(test_port_0), 0);
    ASSERT_EQ(steps[4].port(test_port_0), 1);
    ASSERT_EQ(steps[5].port(test_port_0), 0);
    ASSERT_EQ(steps[6].port(test_port_0), 1);
    ASSERT_EQ(steps[7].port(test_port_0), 0);
}

TEST(TraceBuilder, ShouldRepeatEachStepOfSignal) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10").repeatEachStep(4);

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(test_port_0), 1);
    ASSERT_EQ(steps[1].port(test_port_0), 1);
    ASSERT_EQ(steps[2].port(test_port_0), 1);
    ASSERT_EQ(steps[3].port(test_port_0), 1);
    ASSERT_EQ(steps[4].port(test_port_0), 0);
    ASSERT_EQ(steps[5].port(test_port_0), 0);
    ASSERT_EQ(steps[6].port(test_port_0), 0);
    ASSERT_EQ(steps[7].port(test_port_0), 0);
}

TEST(TraceBuilder, ShouldModifyPortsIndependently) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10").repeatEachStep(2)
        .port(test_port_1).signal("10").repeat(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 4);
    ASSERT_EQ(steps[0].port(test_port_0), 1);
    ASSERT_EQ(steps[1].port(test_port_0), 1);
    ASSERT_EQ(steps[2].port(test_port_0), 0);
    ASSERT_EQ(steps[3].port(test_port_0), 0);
    ASSERT_EQ(steps[0].port(test_port_1), 1);
    ASSERT_EQ(steps[1].port(test_port_1), 0);
    ASSERT_EQ(steps[2].port(test_port_1), 1);
    ASSERT_EQ(steps[3].port(test_port_1), 0);

}

TEST(TraceBuilder, ShouldRepeatForAllPorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10")
        .port(test_port_1).signal("01")
        .allPorts().repeat(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 4);
    ASSERT_EQ(steps[0].port(test_port_0), 1);
    ASSERT_EQ(steps[1].port(test_port_0), 0);
    ASSERT_EQ(steps[2].port(test_port_0), 1);
    ASSERT_EQ(steps[3].port(test_port_0), 0);
    ASSERT_EQ(steps[0].port(test_port_1), 0);
    ASSERT_EQ(steps[1].port(test_port_1), 1);
    ASSERT_EQ(steps[2].port(test_port_1), 0);
    ASSERT_EQ(steps[3].port(test_port_1), 1);
}

TEST(TraeBuilder, ShouldRepeatEachStepForAllPorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10")
        .port(test_port_1).signal("01")
        .allPorts().repeatEachStep(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 4);
    ASSERT_EQ(steps[0].port(test_port_0), 1);
    ASSERT_EQ(steps[1].port(test_port_0), 1);
    ASSERT_EQ(steps[2].port(test_port_0), 0);
    ASSERT_EQ(steps[3].port(test_port_0), 0);
    ASSERT_EQ(steps[0].port(test_port_1), 0);
    ASSERT_EQ(steps[1].port(test_port_1), 0);
    ASSERT_EQ(steps[2].port(test_port_1), 1);
    ASSERT_EQ(steps[3].port(test_port_1), 1);
}
