#include <gmock/gmock.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

namespace {
    PORT_DESCRIPTION(0, test_port_0, 1);
    PORT_DESCRIPTION(1, test_port_1, 1);
    PORT_DESCRIPTION(2, test_port_2, 16);
    PORT_DESCRIPTION(3, test_port_3, 1);

    PORT_DESCRIPTION(1, test_port_1_alternate, 16);
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
    step.port(test_port_0) = true;
    step.port(test_port_3) = false;

    Trace trace;
    trace.append(step);

    // bits 0 and 3 should be set in mask
    // bits 0 = 1, bit 3 = 8
    ASSERT_EQ(0x00000009, trace.getPortMask());
}

TEST(Trace, ShouldGetPortDescription) {
    Step step;
    step.port(test_port_0) = true;
    step.port(test_port_3) = false;

    Trace trace;
    trace.append(step);

    EXPECT_EQ(&test_port_0, &trace.getPortDescription(0));
    EXPECT_EQ(&test_port_3, &trace.getPortDescription(3));
}

TEST(Trace, ShouldFailToGetPortDescriptionForUnknownPort) {
    Step step;
    step.port(test_port_0) = true;
    step.port(test_port_3) = false;

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

TEST(Trace, ShouldConcatenate) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10")
                .port(test_port_1).signal("01");
    
    Trace b = TraceBuilder()
                .port(test_port_0).signal("1")
                .port(test_port_1).signal("0");

    Trace expected = TraceBuilder()
                .port(test_port_0).signal("101")
                .port(test_port_1).signal("010");

    EXPECT_THAT(a+b, MatchesTrace(expected));
}

TEST(Trace, ShouldNotConcatenateTracesWithDifferentNumberOfPorts) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10")
                .port(test_port_1).signal("01");
    
    Trace b = TraceBuilder()
                .port(test_port_0).signal("1")
                .port(test_port_1).signal("0")
                .port(test_port_3).signal("1");

    EXPECT_ANY_THROW( a + b );
}

TEST(Trace, ShouldNotConcatenateTracesWithDifferentPorts) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10")
                .port(test_port_1).signal("01");
    
    Trace b = TraceBuilder()
                .port(test_port_0).signal("1")
                .port(test_port_3).signal("1");

    EXPECT_ANY_THROW( a + b );
}

TEST(Trace, ShouldNotConcatenateTracesWithPortsFromDifferentModules) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10")
                .port(test_port_1).signal("01");
    
    Trace b = TraceBuilder()
                .port(test_port_0).signal("1")
                .port(test_port_1_alternate).signal("1");

    EXPECT_ANY_THROW( a + b );
}

TEST(Trace, ShouldConcatenateEmptyTrace) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10")
                .port(test_port_1).signal("01");
    
    Trace b = TraceBuilder();

    EXPECT_THAT(a+b, MatchesTrace(a));
}

TEST(Trace, ShouldConcatenateToEmptyTrace) {
    Trace a = TraceBuilder();

    Trace b = TraceBuilder()
                .port(test_port_0).signal("10")
                .port(test_port_1).signal("01");
    
    EXPECT_THAT(a+b, MatchesTrace(b));
}

TEST(Trace, ShouldSliceWithinRange) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10110")
                .port(test_port_1).signal("01001");

    Trace b = TraceBuilder()
                .port(test_port_0).signal("10")
                .port(test_port_1).signal("01");
    
    EXPECT_THAT(a.slice(0,2), MatchesTrace(b));
}

TEST(Trace, ShouldSliceOutOfRange) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10110")
                .port(test_port_1).signal("01001");

    Trace b = TraceBuilder();
    
    EXPECT_THAT(a.slice(9,3), MatchesTrace(b));
}

TEST(Trace, ShouldSliceOverlappingRange) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10110")
                .port(test_port_1).signal("01001");

    Trace b = TraceBuilder()
                .port(test_port_0).signal("110")
                .port(test_port_1).signal("001");
    
    EXPECT_THAT(a.slice(2,10), MatchesTrace(b));
}

TEST(Trace, ShouldSliceEntireRange) {
    Trace a = TraceBuilder()
                .port(test_port_0).signal("10110")
                .port(test_port_1).signal("01001");
    
    EXPECT_THAT(a.slice(0,100), MatchesTrace(a));
}

TEST(Trace, ShouldSliceFromEmptyTrace) {
    Trace a;

    EXPECT_THAT(a.slice(0,10), MatchesTrace(a));
}
