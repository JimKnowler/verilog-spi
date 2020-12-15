#include <gmock/gmock.h>

#include "gtest-verilog/MatchesTrace.h"
#include "gtest-verilog/TraceBuilder.h"

using namespace ::testing;

namespace {
    PORT_DESCRIPTION(0, test_port_0, 1);
    PORT_DESCRIPTION(1, test_port_1, 1);
    PORT_DESCRIPTION(2, test_port_2, 1);
}

TEST(MatchesTrace, ShouldMatchEmptyTraces) {
    Trace trace1;
    Trace trace2;
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldMatchSingleOverlappingPortWithSameSignal) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("01");
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldMatchNonOverlappingPortsWithDifferentSignals) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(test_port_2).signal("10");

    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldNotMatchSingleOverlappingPortWithDifferentSignal) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("10");
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

TEST(MatchesTrace, ShouldMatchOverlappingPortsWithSameSignalAndIgnoreNonOverlappingPortsWithDifferentSignals) {
    Trace trace1 = TraceBuilder()       
        .port(test_port_0).signal("00")           // non-overlapping
        .port(test_port_1).signal("10");          // overlapping
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("10")           // overlapping
        .port(test_port_2).signal("11");          // non-overlapping
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldNotMatchOverlappingPortsWithSignalsOfDifferentLength) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("101");
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("10");
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

/// @todo test print nice report to TTY 
///       (by testing underlying matches_trace::compare() directly?)