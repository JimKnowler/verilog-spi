#include <gtest/gtest.h>

#include "TraceBuilder.h"

#include "SPIControllerPort.h"

TEST(TraceBuilder, ShouldConstruct) {
    TraceBuilder builder;
}

TEST(TraceBuilder, ShouldConvertToEmptyTrace) {
    Trace trace = TraceBuilder();

    ASSERT_EQ(trace.getSteps().size(), 0);
}

TEST(TraceBuilder, ShouldAddPort) {
    Trace trace = TraceBuilder()
        .port(i_clk);

    ASSERT_EQ(trace.getSteps().size(), 0);

    // todo: check flag on Trace for which ports have been set
}

TEST(TraceBuilder, ShouldAddPorts) {
    Trace trace = TraceBuilder()
        .port(i_clk)
        .port(o_tx_ready)
        .port(o_spi_clk)
        .port(o_spi_copi);

    ASSERT_EQ(trace.getSteps().size(), 0);

    // todo: check flag on Trace for which ports have been set
}

TEST(TraceBuilder, ShouldAddSignalToPortWithBinarySyntax) {
    Trace trace = TraceBuilder()
        .port(i_clk).signal("10110111");

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(i_clk), 1);
    ASSERT_EQ(steps[1].port(i_clk), 0);
    ASSERT_EQ(steps[2].port(i_clk), 1);
    ASSERT_EQ(steps[3].port(i_clk), 1);
    ASSERT_EQ(steps[4].port(i_clk), 0);
    ASSERT_EQ(steps[5].port(i_clk), 1);
    ASSERT_EQ(steps[6].port(i_clk), 1);
    ASSERT_EQ(steps[7].port(i_clk), 1);
}

TEST(TraceBuilder, ShouldAddSignalToPortWithHiLoSyntax) {
    Trace trace = TraceBuilder()
        .port(i_clk).signal("___-__-_");

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(i_clk), 0);
    ASSERT_EQ(steps[1].port(i_clk), 0);
    ASSERT_EQ(steps[2].port(i_clk), 0);
    ASSERT_EQ(steps[3].port(i_clk), 1);
    ASSERT_EQ(steps[4].port(i_clk), 0);
    ASSERT_EQ(steps[5].port(i_clk), 0);
    ASSERT_EQ(steps[6].port(i_clk), 1);
    ASSERT_EQ(steps[7].port(i_clk), 0);
}

TEST(TraceBuilder, ShouldFailToAddSignalWhenNoPortIsSelected) {
    ASSERT_ANY_THROW(TraceBuilder().signal("1"));
}

TEST(TraceBuilder, ShouldFailToAddSignalWithUnknownCharacter) {
    ASSERT_ANY_THROW(TraceBuilder().port(i_clk).signal("!"));
}

TEST(TraceBuilder, ShouldAddMultipleSignalsToPort) {
    Trace trace = TraceBuilder()
        .port(i_clk).signal("10110").signal("111");

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(i_clk), 1);
    ASSERT_EQ(steps[1].port(i_clk), 0);
    ASSERT_EQ(steps[2].port(i_clk), 1);
    ASSERT_EQ(steps[3].port(i_clk), 1);
    ASSERT_EQ(steps[4].port(i_clk), 0);
    ASSERT_EQ(steps[5].port(i_clk), 1);
    ASSERT_EQ(steps[6].port(i_clk), 1);
    ASSERT_EQ(steps[7].port(i_clk), 1);
}

TEST(TraceBuilder, ShouldAddSignalsToMultiplePorts) {
    Trace trace = TraceBuilder()
             .port(i_clk).signal("__")
        .port(o_tx_ready).signal("_-")
         .port(o_spi_clk).signal("-_")
        .port(o_spi_copi).signal("--");
    
    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 2);
    ASSERT_EQ(steps[0].port(i_clk), 0);
    ASSERT_EQ(steps[1].port(i_clk), 0);
    ASSERT_EQ(steps[0].port(o_tx_ready), 0);
    ASSERT_EQ(steps[1].port(o_tx_ready), 1);
    ASSERT_EQ(steps[0].port(o_spi_clk), 1);
    ASSERT_EQ(steps[1].port(o_spi_clk), 0);
    ASSERT_EQ(steps[0].port(o_spi_copi), 1);
    ASSERT_EQ(steps[1].port(o_spi_copi), 1);
}

TEST(TraceBuilder, ShouldFailToGenerateTraceWhenPortsHaveSignalsWithDifferentLengths) {
    Trace trace;
    ASSERT_ANY_THROW(trace = TraceBuilder()
             .port(i_clk).signal("__")
        .port(o_tx_ready).signal("_")
    );
}

TEST(TraceBuilder, ShouldRepeatSignal) {
    Trace trace = TraceBuilder()
        .port(i_clk).signal("10").repeat(4);

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(i_clk), 1);
    ASSERT_EQ(steps[1].port(i_clk), 0);
    ASSERT_EQ(steps[2].port(i_clk), 1);
    ASSERT_EQ(steps[3].port(i_clk), 0);
    ASSERT_EQ(steps[4].port(i_clk), 1);
    ASSERT_EQ(steps[5].port(i_clk), 0);
    ASSERT_EQ(steps[6].port(i_clk), 1);
    ASSERT_EQ(steps[7].port(i_clk), 0);
}

TEST(TraceBuilder, ShouldRepeatEachStepOfSignal) {
    Trace trace = TraceBuilder()
        .port(i_clk).signal("10").repeatEachStep(4);

    const std::vector<Step>& steps = trace.getSteps();
    ASSERT_EQ(steps.size(), 8);
    ASSERT_EQ(steps[0].port(i_clk), 1);
    ASSERT_EQ(steps[1].port(i_clk), 1);
    ASSERT_EQ(steps[2].port(i_clk), 1);
    ASSERT_EQ(steps[3].port(i_clk), 1);
    ASSERT_EQ(steps[4].port(i_clk), 0);
    ASSERT_EQ(steps[5].port(i_clk), 0);
    ASSERT_EQ(steps[6].port(i_clk), 0);
    ASSERT_EQ(steps[7].port(i_clk), 0);
}

// todo
// - add signals to multiple ports, modifying them independently
// - handle comparing traces where we're only interested in specific ports
//    - flags / mask for ports that have fields
//    - move portId enum to Trace?
// - repeat() applied to entire trace (i.e. .allPorts().repeat(2) )