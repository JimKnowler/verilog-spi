#include <gtest/gtest.h>

#include "TraceBuilder.h"

TEST(TestTraceBuilder, ShouldConstruct) {
    TraceBuilder builder;
}

TEST(TestTraceBuilder, ShouldConvertToEmptyTrace) {
    Trace trace = TraceBuilder();

    ASSERT_EQ(trace.getSteps().size(), 0);
}

TEST(TestTraceBuilder, ShouldAddPort) {
    Trace trace = TraceBuilder()
        .i_clk();

    ASSERT_EQ(trace.getSteps().size(), 0);

    // todo: check flag for which ports have been set
}

TEST(TestTraceBuilder, ShouldAddPorts) {
    Trace trace = TraceBuilder()
        .i_clk()
        .o_tx_ready()
        .o_spi_clk()
        .o_spi_copi();

    ASSERT_EQ(trace.getSteps().size(), 0);

    // todo: check flag for which ports have been set
}

TEST(TestTraceBuilder, ShouldAddSignalToPortWithBinarySyntax) {
    Trace trace = TraceBuilder()
        .i_clk().signal("10110111");

    const std::vector<Trace::Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].i_clk, 1);
    ASSERT_EQ(steps[1].i_clk, 0);
    ASSERT_EQ(steps[2].i_clk, 1);
    ASSERT_EQ(steps[3].i_clk, 1);
    ASSERT_EQ(steps[4].i_clk, 0);
    ASSERT_EQ(steps[5].i_clk, 1);
    ASSERT_EQ(steps[6].i_clk, 1);
    ASSERT_EQ(steps[7].i_clk, 1);
}

TEST(TestTraceBuilder, ShouldAddSignalToPortWithHiLoSyntax) {
    Trace trace = TraceBuilder()
        .i_clk().signal("___-__-_");

    const std::vector<Trace::Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].i_clk, 0);
    ASSERT_EQ(steps[1].i_clk, 0);
    ASSERT_EQ(steps[2].i_clk, 0);
    ASSERT_EQ(steps[3].i_clk, 1);
    ASSERT_EQ(steps[4].i_clk, 0);
    ASSERT_EQ(steps[5].i_clk, 0);
    ASSERT_EQ(steps[6].i_clk, 1);
    ASSERT_EQ(steps[7].i_clk, 0);
}

TEST(TestTraceBuilder, ShouldFailToAddSignalWhenNoPortIsSelected) {
    ASSERT_ANY_THROW(TraceBuilder().signal("1"));
}

TEST(TestTraceBuilder, ShouldFailToAddSignalWithUnknownCharacter) {
    ASSERT_ANY_THROW(TraceBuilder().i_clk().signal("!"));
}

TEST(TestTraceBuilder, ShouldAddMultipleSignalsToPort) {
    Trace trace = TraceBuilder()
        .i_clk().signal("10110").signal("111");

    const std::vector<Trace::Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].i_clk, 1);
    ASSERT_EQ(steps[1].i_clk, 0);
    ASSERT_EQ(steps[2].i_clk, 1);
    ASSERT_EQ(steps[3].i_clk, 1);
    ASSERT_EQ(steps[4].i_clk, 0);
    ASSERT_EQ(steps[5].i_clk, 1);
    ASSERT_EQ(steps[6].i_clk, 1);
    ASSERT_EQ(steps[7].i_clk, 1);
}

TEST(TestTraceBuilder, ShouldAddSignalsToMultiplePorts) {
    Trace trace = TraceBuilder()
             .i_clk().signal("__")
        .o_tx_ready().signal("_-")
         .o_spi_clk().signal("-_")
        .o_spi_copi().signal("--");
    
    const std::vector<Trace::Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 2);
    ASSERT_EQ(steps[0].i_clk, 0);
    ASSERT_EQ(steps[1].i_clk, 0);
    ASSERT_EQ(steps[0].o_tx_ready, 0);
    ASSERT_EQ(steps[1].o_tx_ready, 1);
    ASSERT_EQ(steps[0].o_spi_clk, 1);
    ASSERT_EQ(steps[1].o_spi_clk, 0);
    ASSERT_EQ(steps[0].o_spi_copi, 1);
    ASSERT_EQ(steps[1].o_spi_copi, 1);
}

TEST(TestTraceBuilder, ShouldFailToGenerateTraceWhenPortsHaveSignalsWithDifferentLengths) {
    Trace trace;
    ASSERT_ANY_THROW(trace = TraceBuilder()
             .i_clk().signal("__")
        .o_tx_ready().signal("_")
    );
}

TEST(TestTraceBuilder, ShouldRepeatSignal) {
    Trace trace = TraceBuilder()
        .i_clk().signal("10").repeat(4);

    const std::vector<Trace::Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].i_clk, 1);
    ASSERT_EQ(steps[1].i_clk, 0);
    ASSERT_EQ(steps[2].i_clk, 1);
    ASSERT_EQ(steps[3].i_clk, 0);
    ASSERT_EQ(steps[4].i_clk, 1);
    ASSERT_EQ(steps[5].i_clk, 0);
    ASSERT_EQ(steps[6].i_clk, 1);
    ASSERT_EQ(steps[7].i_clk, 0);
}

TEST(TestTraceBuilder, ShouldRepeatEachStepOfSignal) {
    Trace trace = TraceBuilder()
        .i_clk().signal("10").repeatEachStep(4);

    const std::vector<Trace::Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].i_clk, 1);
    ASSERT_EQ(steps[1].i_clk, 1);
    ASSERT_EQ(steps[2].i_clk, 1);
    ASSERT_EQ(steps[3].i_clk, 1);
    ASSERT_EQ(steps[4].i_clk, 0);
    ASSERT_EQ(steps[5].i_clk, 0);
    ASSERT_EQ(steps[6].i_clk, 0);
    ASSERT_EQ(steps[7].i_clk, 0);
}

// todo
// - add signals to multiple ports, modifying them independently
// - handle comparing traces where we're only interested in specific ports
//    - flags / mask for ports that have fields
//    - move portId enum to Trace?
// - repeat() applied to entire trace