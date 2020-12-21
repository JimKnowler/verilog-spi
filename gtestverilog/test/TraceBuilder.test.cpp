#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

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
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), true);
}

TEST(TraceBuilder, ShouldAddSignalToPortWithHiLoSyntax) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("___-__-_");

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), false);
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
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), true);
}

TEST(TraceBuilder, ShouldAddSignalsToMultiplePorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("__")
        .port(test_port_1).signal("_-")
        .port(test_port_2).signal("-_")
        .port(test_port_3).signal("--");
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 2);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_1)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_1)), true);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_2)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_2)), false);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_3)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_3)), true);
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
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), false);
}

TEST(TraceBuilder, ShouldRepeatEachStepOfSignal) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10").repeatEachStep(4);

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), false);
}

TEST(TraceBuilder, ShouldModifyPortsIndependently) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10").repeatEachStep(2)
        .port(test_port_1).signal("10").repeat(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 4);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_1)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_1)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_1)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_1)), false);

}

TEST(TraceBuilder, ShouldRepeatForAllPorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10")
        .port(test_port_1).signal("01")
        .allPorts().repeat(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 4);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_1)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_1)), true);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_1)), false);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_1)), true);
}

TEST(TraceBuilder, ShouldRepeatEachStepForAllPorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("10")
        .port(test_port_1).signal("01")
        .allPorts().repeatEachStep(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 4);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_1)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_1)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_1)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_1)), true);
}

TEST(TraceBuilder, ShouldIndependentlyModifyMultipleSignalsForSinglePort) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("0").repeat(2).signal("10").repeat(3);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), false);
}

TEST(TraceBuilder, ShouldIndependentlyModifyMultipleSignalsForMultiplePorts) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("0").repeat(2).signal("10").repeatEachStep(2).signal("1").repeat(2)
        .port(test_port_1).signal({3,1}).repeatEachStep(2).signal({2, 4}).repeatEachStep(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), true);

    ASSERT_EQ(std::get<uint32_t>(steps[0].port(test_port_1)), 3);
    ASSERT_EQ(std::get<uint32_t>(steps[1].port(test_port_1)), 3);
    ASSERT_EQ(std::get<uint32_t>(steps[2].port(test_port_1)), 1);
    ASSERT_EQ(std::get<uint32_t>(steps[3].port(test_port_1)), 1);
    ASSERT_EQ(std::get<uint32_t>(steps[4].port(test_port_1)), 2);
    ASSERT_EQ(std::get<uint32_t>(steps[5].port(test_port_1)), 2);
    ASSERT_EQ(std::get<uint32_t>(steps[6].port(test_port_1)), 4);
    ASSERT_EQ(std::get<uint32_t>(steps[7].port(test_port_1)), 4);
}

TEST(TraceBuilder, ShouldConcatenateMultipleSignalsOnSinglePort) {
    Trace trace = TraceBuilder()
        .port(test_port_0).signal("0").signal("0").signal("10").concat().repeat(2);
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(std::get<bool>(steps[0].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[1].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[2].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[3].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[4].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[5].port(test_port_0)), false);
    ASSERT_EQ(std::get<bool>(steps[6].port(test_port_0)), true);
    ASSERT_EQ(std::get<bool>(steps[7].port(test_port_0)), false);
}

TEST(TraceBuilder, ShouldFailToAddEmptyStringSignal) {
    ASSERT_ANY_THROW(
        TraceBuilder()
             .port(test_port_0).signal("")
    );
}

TEST(TraceBuilder, ShouldFailToAddEmptyPortValueSignal) {
    ASSERT_ANY_THROW(
        TraceBuilder()
             .port(test_port_0).signal({})
    );
}
